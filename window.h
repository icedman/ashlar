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

#include "common.h"
#include "iwindow.h"
#include "win32/window.h"

namespace Ash
{
	//! Native win32 window implementation
	class NativeWindow : public OSWin::Window
	{
	public:
		BEGIN_MSG_HANDLER
			HANDLE_MSG(WM_CREATE, OnCreate)
			HANDLE_MSG(WM_DESTROY, OnDestroy)
			HANDLE_MSG(WM_PAINT, OnPaint)
			HANDLE_MSG(WM_SIZE, OnSize)
			HANDLE_MSG(WM_LBUTTONDOWN, OnMouseEvent)
			HANDLE_MSG(WM_RBUTTONDOWN, OnMouseEvent)
			HANDLE_MSG(WM_LBUTTONUP, OnMouseEvent)
			HANDLE_MSG(WM_RBUTTONUP, OnMouseEvent)
			HANDLE_MSG(WM_ERASEBKGND, OnEraseBackground)
			HANDLE_MSG(WM_KEYDOWN, OnKeyEvent)
			END_MSG_HANDLER;

		LRESULT OnCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled );
		LRESULT OnDestroy( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled );
		LRESULT OnEraseBackground( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled );
		LRESULT OnSize( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled );
		LRESULT OnPaint( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled );
		LRESULT OnMouseEvent( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& bHandled );		

	public:
		IWindow *iwindow;
	};

	//! IWindow implementation
	class Window : public IWindow
	{
	public:
		Window();
		~Window();

		//! Create window
		virtual bool Create(int w, int h);
		//! Free window
		virtual void Destroy();

		//! Set size
		virtual bool SetSize(int x, int y, int w, int h);
		//! Show or hide a window
		virtual void Show(bool show);

		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void OnSize(const Rect *rc);
		virtual void OnKeyDown(long key);
		virtual void OnKeyUp(long key);
		virtual void OnMouseMove(Point p);
		virtual void OnMouseLeave();
		virtual void OnMouseDown(int button, Point p);
		virtual void OnMouseUp(int button, Point p);

		// todo: how to remove hdc here?
		virtual void OnDraw(HDC hdc, Rect *rc);

	public:
		NativeWindow window;
	};
}