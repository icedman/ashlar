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

#include <win32/window.h>

using namespace Ash;

namespace OSWin
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
			HANDLE_MSG(WM_MOUSEMOVE, OnMouseEvent)
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

}