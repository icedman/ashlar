
#include "ashlar.h"
#include "list.h"

using namespace Ash;

class Doc : public VFrame
{
public:
	const char* GetName() { return "doc"; }
};

class Label : public Frame
{
public:
	const char* GetName() { return "label"; }
};

class Button : public Frame
{
public:
	bool OnEvent(int eid, void *)
	{
		printf("hit %s\n", GetName());
		return true;
	}
	const char* GetName() { return "button"; }
};

class Spacer : public Frame
{
public:
	const char* GetName() { return "spacer"; }
};

class Toolbar : public HFrame
{
public:
	const char* GetName() { return "toolbar"; }
};

class Client : public HFrame
{
public:
	const char* GetName() { return "client"; }
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
		END_MSG_HANDLER;

	LRESULT OnEraseBackground( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		return 0;
	}

	LRESULT OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{	
		/*
		toolbar.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), UNASSIGNED, 40));
		toolbar.layoutInfo.flex = 1;
		clientArea.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), UNASSIGNED, UNASSIGNED));
		clientArea.layoutInfo.flex = 1;

		doc.AddFrame(&toolbar);
		doc.AddFrame(&clientArea);

		label.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), 200, 40));
		button.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), 60, 30));
		button.layoutInfo.border = 1;

		if (0)
		{
		box.layoutInfo.x = 80;
		box.layoutInfo.y = 80;
		box.layoutInfo.floating = true;
		}
		//spacer.layoutInfo.margin = 0;
		//spacer.layoutInfo.padding = 0;
		spacer.layoutInfo.flex = 1;
		//spacer.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), UNASSIGNED, 40));
		if (1) 
		{
		clientArea.AddFrame(&box2);
		clientArea.AddFrame(&box);
		//clientArea.AddFrame(&spacer);
		box.AddFrame(&label);
		box.AddFrame(&button);
		box.layoutInfo.verticalAlign = MIDDLE;
		box2.layoutInfo.flex = 2;
		}
		clientArea.layoutInfo.align = CENTER;
		clientArea.layoutInfo.verticalAlign = MIDDLE;

		*/

		doc.layoutInfo.align = RIGHT;
		doc.AddFrame(&toolbar);
		doc.AddFrame(&box);
		box.AddFrame(&button);
		box2.AddFrame(&button2);
		toolbar.AddFrame(&box2);
		toolbar.layoutInfo.flex = 1;
	
		button.layoutInfo.border = 1;
		button.layoutInfo.width = 80;
		button.layoutInfo.height = 40;
		button2.layoutInfo.border = 1;
		button2.layoutInfo.width = 80;
		button2.layoutInfo.height = 40;

		Rect r;
		GetWindowRect(m_hWnd, &r);
		doc.SetRect(&r);
		doc.Layout();

		e1.Attach(ONMOUSEDOWN, &button);
		mouseEvents.AddListener(&e1);
		return 0;
	}

	LRESULT OnSize( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		Rect r;
		GetClientRect(m_hWnd, &r);
		doc.SetRect(&r);
		doc.Layout();

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
		mouseEvents.OnMouseEvent(ONMOUSEDOWN, 1, p.x, p.y);
	}

	VOID Draw(HDC hdc, LPRECT rc)
	{
		renderEngine.Clear(RGB(255, 255, 255));
		renderEngine.Render(&doc);
		renderEngine.Blit(hdc);
	}

private:

	Label label;
	Spacer spacer;
	Button button;
	Button button2;
	HFrame box;
	HFrame box2;
	Doc doc;
	Toolbar toolbar;
	Client clientArea;
	RenderEngine renderEngine;
	MouseEvents mouseEvents;
	Event e1;
};

#if 1

int main()
{
	MyWindow win;

	win.RegisterClass(0, _T("Ashlar"));
	win.Create(_T("Ashlar"), _T("Ashlar Prototype"), 0, 0, 600, 400, WS_SIZEBOX | WS_SYSMENU);
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
	DOMDocument doc;
	Element *html = doc.createElement(&DOMString("html"));
	Element *body = doc.createElement(&DOMString("body"));
	body->SetAttribute(&DOMString("background"), &DOMString("#ffffff"));
	doc.AppendChild(html);
	html->AppendChild(body);
	doc.Text();

	doc.FreeNodes(&doc);

	doc.Text();
	return 0;
}

#endif