
#include "ashlar.h"

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
	const char* GetName() { return "button"; }
};

class Spacer : public Frame
{
public:
	const char* GetName() { return "spacer"; }
};

class Toolbar : public VFrame
{
public:
	const char* GetName() { return "toolbar"; }
};

class Client : public VFrame
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
		END_MSG_HANDLER;

	LRESULT OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{	
		toolbar.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), UNASSIGNED, 40));
		toolbar.layoutInfo.flex = 1;
		clientArea.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), UNASSIGNED, UNASSIGNED));
		clientArea.layoutInfo.flex = 1;

		doc.AddFrame(&toolbar);
		doc.AddFrame(&clientArea);

		label.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), 200, 40));
		button.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), 60, 40));
		/*
		box.layoutInfo.x = 80;
		box.layoutInfo.y = 80;
		box.layoutInfo.floating = true;
		*/
		//spacer.layoutInfo.margin = 0;
		//spacer.layoutInfo.padding = 0;
		spacer.layoutInfo.flex = 1;
		//spacer.SetRect(&Rect(Point(UNASSIGNED, UNASSIGNED), UNASSIGNED, 40));
		clientArea.AddFrame(&box2);
		clientArea.AddFrame(&box);
		clientArea.AddFrame(&spacer);
		box.AddFrame(&label);
		box.AddFrame(&button);
		box2.layoutInfo.flex = 2;
		clientArea.layoutInfo.align = CENTER;
		clientArea.layoutInfo.verticalAlign = BOTTOM;

		Rect r;
		GetWindowRect(m_hWnd, &r);
		doc.SetRect(&r);
		doc.Layout();
		return 0;
	}

	LRESULT OnSize( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		Rect r;
		GetClientRect(m_hWnd, &r);
		r.right -= 1;
		r.bottom -= 1;
		doc.SetRect(&r);
		doc.Layout();
		return 0;
	}

	LRESULT OnDestroy( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		PostQuitMessage(0);
		return 0;
	}

	VOID OnLButtonDown(POINT p)
	{
	}

	VOID Draw(HDC hdc, LPRECT rc)
	{
		renderEngine.SetTarget(hdc);
		renderEngine.Render(&doc);
	}

private:

	Label label;
	Spacer spacer;
	Button button;
	VFrame box;
	VFrame box2;
	Doc doc;
	Toolbar toolbar;
	Client clientArea;
	RenderEngine renderEngine;
};

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
