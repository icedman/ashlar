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

#include <widget.h>
#include <layout/label.h>
#include <layout/windowframe.h>
#include <dom/mutationevent.h>
#include <dom/safenode.h>

using namespace Dom;

namespace Layout
{
	Label::Label()
	{
		frameStyle.layout.align = ALIGN_CENTER;
		frameStyle.layout.verticalAlign = ALIGN_MIDDLE;
	}

	bool Label::Prelayout()
	{
		if (frameStyle.layout.flex)
			return true;

		if (ISASSIGNED(frameStyle.layout.width) && ISASSIGNED(frameStyle.layout.height))
			return true;

		WindowFrame *w = (WindowFrame*)GetParent(WINDOW);
		if (w)
		{
			DOMString *text = GetText();
			if (text)
			{
				Render::Rasterizer *r = w->GetRenderer();
				double width, height;
				if (r->GetTextExtents(&frameStyle, text->c_str(), width, height))
				{
					frameStyle.layout.width = ISASSIGNED(frameStyle.layout.width) ? frameStyle.layout.width : width;
					frameStyle.layout.height = ISASSIGNED(frameStyle.layout.height) ? frameStyle.layout.height : height;
				}
			}
		}

		return HFrame::Prelayout();
	}

	DOMString* Label::GetText()
	{
		Element *e = GetElement();
		if (e)
		{
			SafeNode snode(e);
			SafeNode *label = snode.GetAttribute("value");
			if (label->Value())
				return label->Value();
			return &e->nodeValue;
		}
		return 0;
	}

	void Label::Draw(Render::Rasterizer *render)
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
	}

	bool Label::RegisterEventListeners()
	{
		EventTarget *target = (EventTarget*)GetElement();
		target->AddEventListener(MUTATION_EVENTS, ATTRIBUTE_MODIFIED, this, false);
		return Frame::RegisterEventListeners();
	}

	void Label::HandleEvent(Event *evt)
	{
		switch(evt->GetEventGroup())
		{
		case MUTATION_EVENTS:
			{
				switch(evt->type)
				{
				case ATTRIBUTE_MODIFIED:
					{
						MutationEvent *me = (MutationEvent*)evt;
						if (me->relatedNode == GetElement())
						{
							if (me->attrName == "value" || me->attrName == "label")
							{
								Restyle();
								Relayout();
							} else {
								Redraw();
							}
						}
						break;
					}
				}
				break;
			}
		}

		Frame::HandleEvent(evt);
	}
}