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
#include <layout/stylesheet.h>
#include <dom/safenode.h>

namespace Layout
{
	// Frame
	Frame::Frame() 
	{
		frameState = STATE_NORMAL;
		mouseState = STATE_NORMAL;
		parentFrame = 0;
		element = 0;
		SetStyleDefaults(frameStyle);
	}

	Frame::~Frame()
	{
		Free();
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

	}

	// frame tree
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

	// layout
	bool Frame::Prelayout()
	{
		return true;
	}

	bool Frame::Layout()
	{
		Frame *f = frames.GetFirst();
		while(f)
		{
			f->Prelayout();
			f = f->next;
		}
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

	Frame* Frame::GetFrameFromPoint(Point p)
	{
		Frame *f = frames.GetFirst();
		while(f)
		{
			Frame *res = f->GetFrameFromPoint(p);
			if (res)
				return res;
			f = f->next;
		}

		Rect r;
		GetRect(&r);
		if (r.Contains(p))
			return this;

		return 0;
	}

	void Frame::Relayout()
	{
		WindowFrame *w = (WindowFrame*)GetParent(WINDOW);
		if (w)
		{
			w->Relayout();
		}
	}

	// render
	void Frame::Draw(Rasterizer *render)
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
		DrawFrame(render, x, y, x2, y2);

		// draw children
		DrawChildren(render);
	}

	void Frame::DrawFrame(Rasterizer *render, double x, double y, double x2, double y2, DOMString *text)
	{
		Cairo::RefPtr<Cairo::Context> cr = render->GetBufferContext();

		if (!cr)
			return;

		Rect r;
		LayoutInfo *li = &frameStyle.layout;
		Gradient *gr = &frameStyle.gradient;

		double w = x2-x;
		double h = y2-y;

		// fill
		if (gr->colorCount>0)
		{
			cr->save();
			render->DrawBorder(&frameStyle.border, &frameStyle.borderStyle, x, y, x2, y2);
			render->DrawGradient(gr, x, y, x2, y2);
			cr->restore();
		}

		// image
		if (frameStyle.bgImage.imageId)
		{
			cr->save();
			render->DrawBorder(&frameStyle.border, &frameStyle.borderStyle, x, y, x2, y2);
			cr->clip();
			render->DrawImage(&frameStyle.bgImage, x, y, x2, y2);
			cr->restore();
		}

		// text
		if (text)
			DrawFrameText(render, text, x, y, x2, y2);

		// border
		cr->save();
		render->DrawBorder(&frameStyle.border, &frameStyle.borderStyle, x, y, x2, y2);
		cr->stroke();
		cr->restore();
	}

	void Frame::DrawFrameText(Rasterizer *render, DOMString *text, double x, double y, double x2, double y2)
	{
		Cairo::RefPtr<Cairo::Context> cr = render->GetBufferContext();

		if (!cr || !text)
			return;

		if (frameStyle.font.fontId)
		{
			cr->save();
			render->DrawBorder(&frameStyle.border, &frameStyle.borderStyle, x, y, x2, y2);
			cr->clip();
			render->DrawTextLine(&frameStyle.font, &frameStyle.layout, text->c_str(), x, y, x2, y2);
			cr->restore();
		}
	}

	void Frame::DrawChildren(Rasterizer *render)
	{
		// draw children
		Frame *f = frames.GetFirst();
		while(f)
		{
			f->Draw(render);
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

	// dom
	Dom::Element* Frame::SetElement(Dom::Element *e)
	{
		if (!element)
			element = e;
		return element;
	}

	Dom::DOMString* Frame::GetText()
	{
		Element *e = GetElement();
		if (e)
			return &e->nodeValue;
		return 0;
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
}