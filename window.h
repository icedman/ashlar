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
#include <win32/nativewindow.h>

namespace Ash
{
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
		OSWin::NativeWindow window;
	};
}