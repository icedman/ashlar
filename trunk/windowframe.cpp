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

#include "windowFrame.h"

namespace Layout
{
	WindowFrame::WindowFrame()
	{
		frameStyle.layout.floating = true;
	}

	void WindowFrame::OnSize(const Rect *r)
	{
		SetRect(r);
		Layout();

		HDC hdc = GetWindowDC(window.m_hWnd);
		render.InitBuffer(hdc, r);
		ReleaseDC(window.m_hWnd, hdc);
	}

	void WindowFrame::OnDraw(HDC hdc, Rect *rc)
	{
		render.Clear(RGB(255, 255, 255));
		render.Render(this);
		render.Blit(hdc);
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

	void WindowFrame::OnMouseDown(int button, Point p)
	{
		mouseEvents.OnMouseEvent(Events::ONMOUSEDOWN, button, p.x, p.y);
	}

	void WindowFrame::OnMouseUp(int button, Point p)
	{
		mouseEvents.OnMouseEvent(Events::ONMOUSEUP, button, p.x, p.y);
	}

	void WindowFrame::Redraw()
	{
		printf("redraw!\n");
		RedrawWindow(window.m_hWnd, 0, 0, RDW_NOERASE | RDW_INVALIDATE | RDW_INTERNALPAINT);
	}
}