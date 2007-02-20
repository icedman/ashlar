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
#include <cairomm/win32_surface.h>

namespace Layout
{
	WindowFrame::WindowFrame()
	{
	}

	bool WindowFrame::RegisterEvents(Frame *frame)
	{
		frame->RegisterEvents(&mouseEvents);

		FrameList *childFrames = frame->GetFrames();
		Frame *f = childFrames->GetFirst();
		while(f)
		{
			RegisterEvents(f);
			f = f->next;
		}

		return true;
	}

	void WindowFrame::Redraw()
	{
		RedrawWindow(nativeWindow.hWnd, 0, 0, RDW_NOERASE | RDW_INVALIDATE | RDW_INTERNALPAINT);
	}

	// IWindow
	bool WindowFrame::CreateNewWindow(int w, int h)
	{
		DWORD dwStyle = WS_SIZEBOX | WS_SYSMENU;
		//DWORD dwStyle = WS_POPUP;
		nativeWindow.iwindow = static_cast<IWindow*>(this);
		nativeWindow.RegisterClass(0, GetName());
		if (!nativeWindow.Create(GetName(), GetName(),
					200, 0, w, h + 20, dwStyle, 0))
		{
			return false;
		}
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
		Layout();
		render.SetupBuffer(rc->Width(), rc->Height());
	}

	void WindowFrame::OnKeyDown(long key)
	{}

	void WindowFrame::OnKeyUp(long key)
	{}

	void WindowFrame::OnMouseMove(Point p)
	{
		mouseEvents.OnMouseEvent(Events::ONMOUSEMOVE, 0, p.x, p.y);
	}

	void WindowFrame::OnMouseLeave()
	{}

	void WindowFrame::OnMouseDown(int button, Point p)
	{
		mouseEvents.OnMouseEvent(Events::ONMOUSEDOWN, button, p.x, p.y);
	}

	void WindowFrame::OnMouseUp(int button, Point p)
	{
		mouseEvents.OnMouseEvent(Events::ONMOUSEUP, button, p.x, p.y);
	}

	void WindowFrame::OnDraw(HDC hdc, Rect *rc)
	{
		Cairo::RefPtr<Cairo::Win32Surface> surface = Cairo::Win32Surface::create(hdc);
		Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

		if (!cr)
			return;

		render.Clear();
		render.Render(this);
		render.PaintBuffer(cr, rc);
	}
}