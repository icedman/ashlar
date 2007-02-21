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
#include <layout/frames.h>
#include <layout/framestyle.h>
#include <layout/windowframe.h>
#include <events/events.h>
#include <dom/stylesheet.h>
#include <dom/safenode.h>

using namespace Events;

namespace Layout
{
	// Frame
	Frame::Frame() 
	{
		frameState = STATE_NORMAL;
		parentFrame = 0;
		element = 0;
		SetStyleDefaults(frameStyle);
	}

	Frame::~Frame()
	{
		Free();
	}

	bool Frame::SetParent(Frame* parent)
	{
		parentFrame = (Frame*)parent;
		return true;
	}

	Frame* Frame::GetRoot()
	{
		if (parentFrame)
		{
			return parentFrame->GetRoot();
		}
		return 0;
	}

	Frame* Frame::GetParent()
	{
		return parentFrame;
	}

	Frame* Frame::GetParent(int frameType)
	{
		if (parentFrame)
		{
			if (parentFrame->IsType(frameType))
				return parentFrame;
			return parentFrame->GetParent(frameType);
		}
		return 0;
	}

	void Frame::SetState(int state)
	{
		if (!element || frameState == state)
			return;

		frameState = state;
		Widget *w = (Widget*)GetParent(WIDGET);
		if (!w)
			return;

		switch(frameState)
		{
		case STATE_PRESSED:
			element->SetAttribute(&DOMString("#pseudoClass"),&DOMString("pressed"));
			break;
		case STATE_HOVER:
			element->SetAttribute(&DOMString("#pseudoClass"),&DOMString("hover"));
			break;
		default:
			element->SetAttribute(&DOMString("#pseudoClass"),&DOMString(""));
			break;
		}

		StyleSheet *css = w->GetStyleSheet();
		if (css)
			css->ApplyStyle(element);

		Redraw();
	}

	bool Frame::AddFrame(Frame* pFrame)
	{
		frames.Push(pFrame);
		pFrame->SetParent(this);
		return true;
	}

	bool Frame::RemoveFrame(Frame* pFrame)
	{
		Frame *frame = frames.GetFirst();
		while(frame)
		{
			if (frame == pFrame)
			{
				frames.Remove(frame);
				frame->SetParent(0);
				break;
			}
			frame = frame->next;
		}
		return true;
	}

	Dom::DOMString* Frame::GetText()
	{
		DOMString *text = 0;
		Element *e = GetElement();
		if (!e)
			return 0;
		if (IsType(LABEL))
		{
			text = &e->nodeValue;
		} else {
			SafeNode snode(e);
			SafeNode *label = snode.GetValue("label");
			text = label->Value();
		}
		return text;
	}

	bool Frame::Layout()
	{
		//printf("layout %s\n", GetName());
		return true;
	}

	bool Frame::OnEvent(int eventId, void *)
	{
		return true;
	}

	bool Frame::GetRect(Rect *pRect)
	{
		(*pRect) = frameStyle.layout.rect;
		return true;
	}

	bool Frame::GetBorderRect(Rect *pRect)
	{
		(*pRect) = frameStyle.layout.rect;
		int mgl, mgt, mgr, mgb;
		GetBorders(frameStyle.margin, mgl, mgt, mgr, mgb);
		pRect->left += mgl;
		pRect->top += mgt;
		pRect->right -= mgr;
		pRect->bottom -= mgb;
		return true;
	}

	bool Frame::GetContentRect(Rect *pRect)
	{
		(*pRect) = frameStyle.layout.rect;
		int ol, ot, or, ob;
		GetContentOffsets(frameStyle, ol, ot, or, ob);
		pRect->left += ol;
		pRect->top += ot;
		pRect->right -= or;
		pRect->bottom -= ob;
		return true;
	}

	bool Frame::SetRect(const Rect* pRect)
	{
		frameStyle.layout.x = pRect->left;
		frameStyle.layout.y = pRect->top;
		frameStyle.layout.width = pRect->Width();
		frameStyle.layout.height = pRect->Height();
		frameStyle.layout.rect = (*pRect);
		return true;
	}

	void Frame::Free()
	{
		if (parentFrame)
		{
			parentFrame->RemoveFrame(this);
		}

		// free child frames
		Frame *f;
		while(f = frames.GetFirst())
		{
			delete f;
		}
	}

	void Frame::Dump()
	{
		int level = 0;
		Frame *f = parentFrame;
		while(f)
		{
			f = f->parentFrame;
			level++;
		}

		for(int i = 0; i<level; i++) printf("  ");
		printf("%s %d\n", GetName(), GetElement());

		f = frames.GetFirst();
		while(f)
		{
			f->Dump();
			f = f->next;
		}
	}

	void Frame::Redraw()
	{
		WindowFrame *w = (WindowFrame*)GetParent(WINDOW);
		if (w)
		{
			w->Redraw();
		}
	}

	Dom::Element* Frame::SetElement(Dom::Element *e)
	{
		if (!element)
			element = e;
		return element;
	}

	bool Frame::OnMouseEvents(int eid, void *pp)
	{
		MouseInfo *mInfo = (MouseInfo*)pp;

		Rect r;
		GetRect(&r);

		if (!r.Contains(mInfo->point))
		{
			if (GetState() == STATE_PRESSED || GetState() == STATE_HOVER)
				SetState(STATE_NORMAL);
			return true;
		}

		switch(eid)
		{
		case ONMOUSEMOVE:
			if (GetState() != STATE_PRESSED && GetState() != STATE_HOVER)
				SetState(STATE_HOVER);
			break;
		case ONMOUSEDOWN:
			SetState(STATE_PRESSED);
			break;
		case ONMOUSEUP:
			if (GetState() == STATE_PRESSED)
			{
				OnMouseEvents(ONMOUSECLICK, pp);
			}
			SetState(STATE_HOVER);
			break;
		case ONMOUSECLICK:
			printf("unfreed objects:%d\n", Ref::GetCount());
			SafeNode snode(GetElement());
			DOMString *script = snode.GetElement("event")->Value();
			if (script)
			{
				Widget *w = (Widget*)GetParent(WIDGET);
				if (w)
				{
					ScriptEngine *s = w->GetScriptEngine();
					if (s)
						s->RunScript(script->c_str(), script->size());
				}
			}
			break;
		}
		return true;
	}

	bool Frame::OnKeyEvents(int eid, void *pp)
	{
		return true;
	}
}