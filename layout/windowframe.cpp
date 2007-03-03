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

#include <layout/windowframe.h>
#include <dom/uievent.h>
#include <dom/safenode.h>
#include <cairomm/win32_surface.h>

using namespace Dom;

#define TRANS 0
#define WINDOW_CLASS _T("Ashlar")

namespace Layout
{

	WindowFrame::WindowFrame()
	{
		dirtyLayout = true;
	}

	bool WindowFrame::Initialize()
	{
		LayoutInfo *li = &frameStyle.layout;
		li->x = !ISASSIGNED(li->x) ? 0 : li->x;
		li->y = !ISASSIGNED(li->y) ? 0 : li->y;
		li->width = !ISASSIGNED(li->width) ? 100 : li->width;
		li->height = !ISASSIGNED(li->height) ? 100 : li->height;

		if (!CreateNewWindow(li->x, li->y, li->width, li->height))
			return false;

		Frame::Initialize();
		ShowWindow(true);	
		
		return true;
	}

	void WindowFrame::Redraw()
	{
		nativeWindow.Redraw();
	}

	// IWindow
	bool WindowFrame::CreateNewWindow(int x, int y, int w, int h)
	{
		if (nativeWindow.hWnd)
			return true;

		DWORD dwStyle = WS_SIZEBOX | WS_SYSMENU;
		if (TRANS)
			dwStyle = WS_POPUP;

		nativeWindow.iwindow = static_cast<IWindow*>(this);
		nativeWindow.RegisterClass(0, WINDOW_CLASS);
		if (!nativeWindow.Create(WINDOW_CLASS, WINDOW_CLASS,
			x, y, w, h, dwStyle, 0))
			return false;

		return true;
	}

	void WindowFrame::DestroyWindow()
	{
		nativeWindow.DestroyWindow();
	}

	bool WindowFrame::SetWindowSize(int x, int y, int w, int h)
	{
		return true;
	}

	void WindowFrame::ShowWindow(bool show)
	{
		nativeWindow.ShowWindow(show ? SW_SHOWNA : SW_HIDE);
	}

	bool WindowFrame::OnCreate()
	{
		return true;
	}

	void WindowFrame::OnDestroy()
	{}

	void WindowFrame::OnSize(const Rect *rc)
	{
		SetRect(rc);
		render.SetupBuffer(rc->Width(), rc->Height());
		Layout();
	}

	void WindowFrame::OnKeyEvent(int eventId, long key)
	{}

	void WindowFrame::OnMouseEvent(int eventId, int button, Point p)
	{
		MouseEvent me;
		me.InitEvent(
			eventId, // eventTypeArg, 
			true, // canBubbleArg,
			false, // cancelableArg,
			p.x,   // screenX,
			p.y,   // screenY,
			p.x,   // clientX,
			p.y,   // clientY,
			false, // ctrlKey,
			false, // shiftKey,
			false, // altKey,
			false, // metaKey,
			button // button
			);
		PropagateMouseEvent(&me);
	}

	void WindowFrame::OnDraw(HDC hdc, Rect *rc)
	{
		if (dirtyLayout)
		{
			dirtyLayout = false;
			Layout();
		}

		if (TRANS)
		{
			render.Clear();
			Draw(&render);
			nativeWindow.SetAlphaChannel(render.GetBuffer());
		} else {
			Cairo::RefPtr<Cairo::Win32Surface> surface = Cairo::Win32Surface::create(hdc);
			Cairo::RefPtr<Cairo::Context> cx = Cairo::Context::create(surface);
			if (!cx)
				return;
			render.Clear(1, 1, 1, 1);
			Draw(&render);
			render.PaintBuffer(cx, rc);
		}

		printf(".");
	}

	void WindowFrame::Relayout()
	{
		dirtyLayout = true;
		Redraw();
	}
}