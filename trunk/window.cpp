/*
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"){} you may not use this file except in compliance with
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

#pragma once

#include "window.h"
#include "events.h"

namespace Ash
{
	LRESULT NativeWindow::OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		iwindow->OnCreate();
		return 0;
	}

	LRESULT NativeWindow::OnDestroy( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		iwindow->OnDestroy();
		PostQuitMessage(0);
		return 0;
	}

	LRESULT NativeWindow::OnEraseBackground( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		return 0;
	}

	LRESULT NativeWindow::OnSize( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		RECT r;
		GetClientRect(m_hWnd, &r);
		iwindow->OnSize(&Rect(r.left, r.top, r.right, r.bottom));
		return 0;
	}

	LRESULT NativeWindow::OnPaint( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd,&ps);
		iwindow->OnDraw(ps.hdc, (Rect*)&ps.rcPaint);
		EndPaint(m_hWnd,&ps);
		return 0;
	}

	LRESULT NativeWindow::OnMouseEvent( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		Point p;
		p.x=LOWORD(lparam);
		p.y=HIWORD(lparam);

		switch(msg)
		{
		case WM_LBUTTONDOWN:
			iwindow->OnMouseDown(Events::LBUTTON, p);
			break;
		case WM_RBUTTONDOWN:
			iwindow->OnMouseDown(Events::RBUTTON, p);
			break;
		case WM_LBUTTONUP:
			iwindow->OnMouseUp(Events::LBUTTON, p);
			break;
		case WM_RBUTTONUP:
			iwindow->OnMouseUp(Events::RBUTTON, p);
			break;
		case WM_MOUSEMOVE:
			iwindow->OnMouseMove(p);
			break;
		}
		return 0;
	}

	Window::Window()
	{
		window.iwindow = this;
	}

	Window::~Window()
	{
		window.DestroyWindow();
	}

	bool Window::Create(int w, int h)
	{
		window.RegisterClass(0, _T("Ashlar"));
		window.Create(_T("Ashlar"), _T("Ashlar Prototype"), w, h, 200, 200, WS_SIZEBOX | WS_SYSMENU);
		window.ShowWindow(SW_SHOW);
		return true;
	}

	void Window::Destroy()
	{
	}

	void Window::OnSize(const Rect *rc)
	{
	}

	bool Window::SetSize(int x, int y, int w, int h)
	{
		Rect r(x, y, w, h);
		return true;
	}

	void Window::Show(bool show)
	{
		window.ShowWindow(show ? SW_SHOWNA : SW_HIDE);
	}

	bool Window::OnCreate()
	{
		return true;
	}

	void Window::OnDestroy()
	{
	}

	void Window::OnKeyDown(long key)
	{
	}

	void Window::OnKeyUp(long key)
	{
	}

	void Window::OnMouseMove(Point p)
	{
	}

	void Window::OnMouseLeave()
	{
	}

	void Window::OnMouseDown(int button, Point p)
	{
	}

	void Window::OnMouseUp(int button, Point p)
	{
	}

	void Window::OnDraw(HDC hdc, Rect *rc)
	{
	}
}