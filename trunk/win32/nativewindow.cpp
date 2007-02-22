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
#include <cairomm/context.h>
#include <cairomm/win32_surface.h>


namespace OSWin
{
	LRESULT NativeWindow::OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
	{
		// init transparency

		pSetLayeredWindowAttributes = 0;
		pUpdateLayeredWindow = 0;

		OSVERSIONINFO os = { sizeof(os) };
		GetVersionEx(&os);
		BOOL isWin2k = ( VER_PLATFORM_WIN32_NT == os.dwPlatformId && os.dwMajorVersion >= 5 ); 

		if (isWin2k)
		{
			HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
			pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, 
				"SetLayeredWindowAttributes");

			pUpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, 
				"UpdateLayeredWindow");
		}

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
			SetFocus(hWnd);
			iwindow->OnMouseMove(p);
			break;
		}
		return 0;
	}

	BOOL NativeWindow::DragWindow()
	{
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION,NULL);
		return 1;
	}

	BOOL NativeWindow::SetAlphaChannel(Cairo::RefPtr<Cairo::Surface> alpha)
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

		//pSetLayeredWindowAttributes(hWnd, RGB(255,0,255), (255 / 50) * 100, LWA_COLORKEY);

		Rect r;
		GetWindowRect(hWnd, &r);

		BLENDFUNCTION blend;
		blend.AlphaFormat = AC_SRC_ALPHA;
		blend.BlendFlags = 0;
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;

		Cairo::RefPtr<Cairo::Win32Surface> surface = Cairo::Win32Surface::create(Cairo::FORMAT_ARGB32, r.Width(), r.Height());
		Cairo::RefPtr<Cairo::Context> cx = Cairo::Context::create(surface);

		cx->save();
		cx->set_source(alpha, 0, 0);
		cx->rectangle(0, 0, r.Width(), r.Height());
		cx->fill();
		cx->restore();

		HDC screendc = GetDC(0);
		HDC memdc = surface->get_dc();

		//BitBlt(screendc, 0, 0, 50, 50, memdc, 0, 0, SRCCOPY);

		POINT ptSrc = {0, 0};
		POINT ptDst = {r.left, r.top};
		SIZE  sz = {r.Width(), r.Height()};

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