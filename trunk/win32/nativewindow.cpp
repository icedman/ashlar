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

#include <win32/nativewindow.h>
#include <events/events.h>

namespace OSWin
{

	LRESULT NativeWindow::OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		if (!iwindow)
			return 0;

		iwindow->OnCreate();
		return 0;
	}

	LRESULT NativeWindow::OnDestroy( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		if (!iwindow)
			return 0;

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
		if (!iwindow)
			return 0;

		RECT r;
		GetClientRect(hWnd, &r);
		iwindow->OnSize(&Rect(r.left, r.top, r.right, r.bottom));
		return 0;
	}

	LRESULT NativeWindow::OnPaint( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		if (!iwindow)
			return 0;

		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);
		iwindow->OnDraw(ps.hdc, (Rect*)&ps.rcPaint);
		EndPaint(hWnd,&ps);
		return 0;
	}

	LRESULT NativeWindow::OnMouseEvent( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		if (!iwindow)
			return 0;

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

}