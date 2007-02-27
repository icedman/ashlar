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

#include <layout/button.h>
#include <layout/windowframe.h>
#include <layout/stylesheet.h>
#include <dom/uievent.h>
#include <dom/safenode.h>

using namespace Dom;
using namespace Render;

namespace Layout
{
	Button::Button()
	{
		frameStyle.layout.align = ALIGN_CENTER;
		frameStyle.layout.verticalAlign = ALIGN_MIDDLE;
	}

	DOMString* Button::GetText()
	{
		Element *e = GetElement();
		if (e)
		{
			SafeNode snode(e);
			SafeNode *label = snode.GetAttribute("label");
			return label->Value();
		}
		return 0;
	}

	void Button::Draw(Rasterizer *render)
	{
		LayoutInfo *li = &frameStyle.layout;
		bool draw = true;

		Rect r;
		GetRect(&r);

		// skip invisible
		draw &= (li->visible && li->display);
		draw &= (r.Width() > 0 && r.Height() > 0);

		if (!draw)
			return;

		double x, y, x2, y2;

		GetBorderRect(&r);
		x = r.left;
		y = r.top;
		x2 = x + r.Width();
		y2 = y + r.Height();

		// draw basic frame
		DrawFrame(render, x, y, x2, y2, GetText());

		// draw children
		DrawChildren(render);
	}

	bool Button::RegisterEventListeners()
	{
		EventTarget *target = (EventTarget*)GetElement();
		target->AddEventListener(MOUSE_EVENTS, MOUSE_CLICK, this, true);
		target->AddEventListener(MOUSE_EVENTS, MOUSE_DOWN, this, true);
		target->AddEventListener(MOUSE_EVENTS, MOUSE_UP, this, true);
		target->AddEventListener(MOUSE_EVENTS, MOUSE_MOVE, this, true);
		target->AddEventListener(MOUSE_EVENTS, MOUSE_OVER, this, true);
		target->AddEventListener(MOUSE_EVENTS, MOUSE_OUT, this, true);
		return Label::RegisterEventListeners();
	}

	void Button::HandleEvent(Event *evt)
	{
		switch(evt->GetEventGroup())
		{
		case MOUSE_EVENTS:
			{
				switch(evt->type)
				{
				case MOUSE_DOWN:
					{
						SetState(STATE_PRESSED);
						break;
					}
				case MOUSE_UP:
					{
						SetState(STATE_NORMAL);
						break;
					}
				case MOUSE_CLICK:
					{
						SetState(STATE_HOVER);
						break;
					}
				case MOUSE_OVER:
					{
						SetState(STATE_HOVER);
						break;
					}
				case MOUSE_MOVE:
					{
						break;
					}
				case MOUSE_OUT:
					{
						SetState(STATE_NORMAL);
						break;
					}
				}
				break;
			}
		}

		Label::HandleEvent((MouseEvent*)evt);
	}
}