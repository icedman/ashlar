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
#include <cairomm/context.h>
#include <cairomm/win32_surface.h>

namespace PlatformDependent
{

#define IDT_REDRAW 0xf001

	LRESULT NativeWindow::OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		// init transparency

		pSetLayeredWindowAttributes = 0;
		pUpdateLayeredWindow = 0;
		pTrackMouseEvent = 0;

		OSVERSIONINFO os = { sizeof(os) };
		GetVersionEx(&os);
		BOOL isWin2k = ( VER_PLATFORM_WIN32_NT == os.dwPlatformId && os.dwMajorVersion >= 5 ); 
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));

		if (isWin2k)
		{
			pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, 
				"SetLayeredWindowAttributes");

			pUpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, 
				"UpdateLayeredWindow");
		}

		pTrackMouseEvent = (lpfnTrackMouseEvent)GetProcAddress(hUser32, 
			"TrackMouseEvent");

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
			{
				iwindow->OnMouseEvent(MOUSE_DOWN, LEFT_BUTTON, p);
				break;
			}
		case WM_RBUTTONDOWN:
			{
				iwindow->OnMouseEvent(MOUSE_DOWN, RIGHT_BUTTON, p);
				break;
			}
		case WM_LBUTTONUP:
			{
				iwindow->OnMouseEvent(MOUSE_UP, LEFT_BUTTON, p);
				break;
			}
		case WM_RBUTTONUP:
			{
				iwindow->OnMouseEvent(MOUSE_UP, RIGHT_BUTTON, p);
				break;
			}
		case WM_MOUSELEAVE:
			{
				iwindow->OnMouseEvent(MOUSE_OUT, 0, p);
				break;
			}
		case WM_MOUSEMOVE:
			{
				TrackMouse();
				iwindow->OnMouseEvent(MOUSE_MOVE, 0, p);
				break;
			}
		}
		return 0;
	}

	LRESULT NativeWindow::OnKeyEvent( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		PostQuitMessage(0);
		return 0;
	}

	LRESULT NativeWindow::OnTimer( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		switch(wparam)
		{
		case IDT_REDRAW:
			{
				RedrawWindow(hWnd, 0, 0, RDW_NOERASE | RDW_INVALIDATE | RDW_INTERNALPAINT);
				KillTimer(hWnd, IDT_REDRAW);
				break;
			}
		}
		return 0;
	}

	BOOL NativeWindow::DragWindow()
	{
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION,NULL);
		return 1;
	}

	BOOL NativeWindow::Redraw()
	{
		//RedrawWindow(hWnd, 0, 0, RDW_NOERASE | RDW_INVALIDATE | RDW_INTERNALPAINT);
		KillTimer(hWnd, IDT_REDRAW);
		SetTimer(hWnd, IDT_REDRAW, 10, 0);
		return 1;
	}

	BOOL NativeWindow::TrackMouse()
	{
		if (pTrackMouseEvent)
		{
			TRACKMOUSEEVENT tm;
			tm.cbSize = sizeof(TRACKMOUSEEVENT);
			tm.dwFlags = TME_QUERY;
			tm.hwndTrack = hWnd;
			pTrackMouseEvent(&tm);

			if (!(tm.dwFlags & TME_LEAVE))
			{
				tm.dwFlags = TME_LEAVE;
				tm.hwndTrack = hWnd;
				pTrackMouseEvent(&tm);
			}
		}
		return TRUE;
	}

	BOOL NativeWindow::SetAlphaChannel(Cairo::RefPtr<Cairo::Surface> alpha, int trans)
	{
		if (!pSetLayeredWindowAttributes ||	!pUpdateLayeredWindow)
			return 0;

		DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

		if (!alpha)
		{	
			dwExStyle = dwExStyle &~ WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
			return true;
		} else {
			dwExStyle = dwExStyle | WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
		}

		RECT r;
		GetWindowRect(hWnd, &r);

		BLENDFUNCTION blend;
		blend.AlphaFormat = AC_SRC_ALPHA;
		blend.BlendFlags = 0;
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = trans;

		Cairo::RefPtr<Cairo::Win32Surface> surface = Cairo::Win32Surface::create(Cairo::FORMAT_ARGB32, r.right - r.left, r.bottom - r.top);
		Cairo::RefPtr<Cairo::Context> cx = Cairo::Context::create(surface);

		cx->save();
		cx->set_source(alpha, 0, 0);
		cx->rectangle(0, 0, r.right - r.left, r.bottom - r.top);
		cx->fill();
		cx->restore();

		HDC screendc = GetDC(0);
		HDC memdc = surface->get_dc();

		POINT ptSrc = {0, 0};
		POINT ptDst = {r.left, r.top};
		SIZE  sz = { r.right - r.left, r.bottom - r.top};

		pUpdateLayeredWindow(
			hWnd,
			screendc,
			&ptDst,
			&sz,
			memdc,
			&ptSrc,
			0,
			&blend,
			LWA_ALPHA
			);

		ReleaseDC(0, screendc);
		return 1;
	}

}