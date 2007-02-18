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

#pragma once

#include <common.h>
#include <iwindow.h>

// ms windows
#include <tchar.h>
#include <windows.h>

using namespace Ash;

namespace OSWin
{

	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define BEGIN_MSG_HANDLER \
	LRESULT CALLBACK WindowsProc(UINT uMsg, WPARAM wParam, LPARAM lParam) { \
	LRESULT result; \
	BOOL bHandled=TRUE;

#define HANDLE_MSG(msg,func) \
	if (uMsg==msg)  { \
	result=func(uMsg,wParam,lParam,bHandled); \
	if (bHandled) { return result; }\
	}
#define CHAIN_MSG(func) \
	result=func(uMsg,wParam,lParam,bHandled); \
	if (bHandled) { return result; }

#define END_MSG_HANDLER \
	return ( DefWindowProc( hWnd, uMsg, wParam, lParam )); }

	//! Native window base class
	/*!
	Win32 native window
	*/
	class Window
	{
	public:
		Window()
		{
			hWnd=NULL;
		}

		~Window()
		{

		}

		BOOL RegisterClass(HINSTANCE hInstance, const TCHAR* lpszClass)
		{
			hInstance=hInstance;

			WNDCLASS wc;

			wc.lpszClassName = lpszClass;
			wc.lpfnWndProc = WinProc;
			wc.style = CS_VREDRAW | CS_HREDRAW;
			wc.hInstance = hInstance;
			wc.hIcon = NULL;
			wc.hCursor = LoadCursor( NULL, IDC_ARROW );
			wc.lpszMenuName = NULL;
			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;

			return (::RegisterClass(&wc)!=NULL);
		}

		VOID UnregisterClass(const TCHAR* lpszClass)
		{
			::UnregisterClass(lpszClass,0);
		}

		BOOL Create(const TCHAR* lpszClass,const TCHAR* lpszTitle,int x,int y,int w,int h,DWORD style,DWORD exStyle=0,HWND hwndParent=NULL)
		{
			hWnd = CreateWindowEx( exStyle, lpszClass, lpszTitle,style,
				x, y,
				w, h,
				hwndParent,
				NULL,
				hInstance,
				(LPVOID)this);
			return (hWnd!=NULL);
		}


		VOID UpdateWindow()
		{
			::UpdateWindow(hWnd);
		}

		VOID ShowWindow(int cmd = SW_SHOW)
		{
			::ShowWindow(hWnd,cmd);
		}

		VOID DestroyWindow()
		{
			::DestroyWindow(hWnd);
		}

		virtual BOOL OnCreate() { return TRUE; }
		virtual VOID OnKeyDown(LONG key) { PostQuitMessage(0); }
		virtual VOID OnKeyUp(LONG key) {}
		virtual VOID OnDraw(HDC hdc,LPRECT rc) {}

		virtual LRESULT CALLBACK WindowsProc(UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc( hWnd, uMsg, wParam, lParam ); }

	protected:

		virtual LRESULT OnKeyEvent( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled )
		{
			LONG key = (LONG)wparam;
			switch(msg) {
	case WM_KEYDOWN:
		OnKeyDown(key);
		break;
	case WM_KEYUP:
		OnKeyUp(key);
		break;
			}
			return 0;
		}

	public:
		HWND hWnd;
		HINSTANCE hInstance;
	};

	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window *w=NULL;
		if (uMsg==WM_CREATE) {
			LPCREATESTRUCT cs=(LPCREATESTRUCT)lParam;
			w=reinterpret_cast<Window*>(cs->lpCreateParams);
			w->hWnd=hWnd;
			SetWindowLong(hWnd,GWL_USERDATA,(LONG)w);
			w->OnCreate();
		} else {
			w=reinterpret_cast<Window*>(GetWindowLong(hWnd,GWL_USERDATA));
		}
		if (w) {
			w->hWnd = hWnd;
			return (w->WindowsProc(uMsg,wParam,lParam));
		}
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
}