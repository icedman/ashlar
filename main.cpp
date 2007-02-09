/*
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Copyright 2007
Marvin Sanchez
code.google.com/p/ashlar
*/

// Note: this is a demo program. it leaks!

#include "ashlar.h"

using namespace Ash;
using namespace Dom;
using namespace Layout;
using namespace Render;
using namespace Events;

class Doc : public VFrame
{
public:
	const char* GetName() { return "window"; }
	Frame* Create() { return new Doc(); }
};

class Label : public Frame
{
public:
	const char* GetName() { return "label"; }
	Frame* Create() { return new Label(); }
};

class Button : public Frame
{
public:
	Button()
	{
		frameStyle.border.width = 1;
	}

	bool OnEvent(int eid, void *)
	{
		printf("hit %s\n", GetName());
		return true;
	}
	const char* GetName() { return "button"; }
	Frame* Create() { return new Button(); }
};

class Spacer : public Frame
{
public:
	const char* GetName() { return "spacer"; }
	Frame* Create() { return new Spacer(); }
};

class Toolbar : public HFrame
{
public:
	const char* GetName() { return "toolbar"; }
	Frame* Create() { return new Toolbar(); }
};

class Client : public HFrame
{
public:
	const char* GetName() { return "client"; }
	Frame* Create() { return new Client(); }
};

class MyWindow : public Window
{
	BEGIN_MSG_HANDLER
		HANDLE_MSG(WM_PAINT, OnPaint)
		HANDLE_MSG(WM_CREATE, OnCreate)
		HANDLE_MSG(WM_DESTROY, OnDestroy)
		HANDLE_MSG(WM_SIZE, OnSize)
		HANDLE_MSG(WM_LBUTTONDOWN, OnMouseEvent)
		HANDLE_MSG(WM_ERASEBKGND, OnEraseBackground)
		HANDLE_MSG(WM_KEYDOWN, OnKeyEvent)
		END_MSG_HANDLER;

	LRESULT OnEraseBackground( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		return 0;
	}

	LRESULT OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		doc = 0;
		domdoc = 0;

		DOMBuilder p;
		p.Initialize();

		FILE *fp = fopen("ashlar.xul", "r");
		while(!feof(fp))
		{
			char buffer[1024];
			unsigned short len = fread(buffer, 1, 1024, fp);
			int isFinal = feof(fp);
			p.Parse(buffer, len, isFinal);
		}
		fclose(fp);

		domdoc = p.GetDocument();
		p.Shutdown();

		if (!domdoc)
		{
			return 0;
		}

		domdoc->Dump();

		FrameBuilder fb;
		fb.Register(new Frame());
		fb.Register(new HFrame());
		fb.Register(new VFrame());
		fb.Register(new Doc());
		fb.Register(new Label());
		fb.Register(new Button());
		fb.Build(domdoc);

		doc = (Doc*)fb.root;
		if (!doc)
			return 0;

		doc->Dump();

		if (0)
		{
			/*
			hack:
			layout doesn't compute properly at a few passes when frames have no widths & heights and also no flex values
			*/
			Rect r;
			GetClientRect(m_hWnd, &r);
			doc->SetRect(&r);
			for(int i=0;i<8;i++)
				doc->Layout();
		}

		return 0;
	}

	LRESULT OnSize( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		if (!doc)
			return 0;

		Rect r;
		GetClientRect(m_hWnd, &r);
		doc->SetRect(&r);
		doc->Layout();

		HDC hdc = GetWindowDC(m_hWnd);
		renderEngine.InitBuffer(hdc, &r);
		ReleaseDC(m_hWnd, hdc);
		return 0;
	}

	LRESULT OnDestroy( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		PostQuitMessage(0);
		return 0;
	}

	VOID OnLButtonDown(POINT p)
	{
		// mouseEvents.OnMouseEvent(ONMOUSEDOWN, 1, p.x, p.y);
	}


	VOID Draw(HDC hdc, LPRECT rc)
	{
		if (!doc)
			return;

		renderEngine.Clear(RGB(255, 255, 255));
		renderEngine.Render(doc);
		renderEngine.Blit(hdc);
	}

private:

	Doc *doc;
	DOMDocument *domdoc;
	RenderEngine renderEngine;
};

#if 1

int main()
{
	MyWindow win;

	win.RegisterClass(0, _T("Ashlar"));
	win.Create(_T("Ashlar"), _T("Ashlar Prototype"), 400, 200, 200, 200, WS_SIZEBOX | WS_SYSMENU);
	win.ShowWindow(SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

#else

int main()
{

	return 0;
}

#endif