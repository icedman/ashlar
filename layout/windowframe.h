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

#include <layout/frames.h>
#include <layout/layout.h>
#include <render/render.h>
#include <layout/frametypes.h>
#include <iwindow.h>
#include <win32/nativewindow.h>

namespace Layout
{
	class WindowFrame : public VFrame, public Ash::IWindow
	{
	public:
		WindowFrame();
		virtual ~WindowFrame() {}

		const char* GetName() { return "window"; }
		Frame* Create() { return new WindowFrame(); }
		FRAMETYPE(WINDOW, VFrame)

			virtual bool Initialize();
		virtual void Redraw();

		//! IWindow Interface
		virtual bool CreateNewWindow(int x, int y, int w, int h, IWindow* parent = 0);
		virtual void DestroyWindow();
		virtual bool SetWindowSize(int x, int y, int w, int h);
		virtual void ShowWindow(bool show);
		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void OnSize(const Rect *rc);
		virtual void OnKeyEvent(int eventId, long key);
		virtual void OnMouseEvent(int eventId, int button, Point p);
		virtual void OnDraw(HDC hdc, Rect *rc);

		virtual void Relayout();

		Render::Rasterizer* GetRenderer() { return &render; }

	private:
		Render::Rasterizer render;
		bool dirtyLayout;

		PlatformDependent::NativeWindow nativeWindow;
	};
}