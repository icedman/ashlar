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

#include "frames.h"

namespace Layout
{
	// FrameTool
	void FrameTool::SetDefaults(LayoutInfo &layoutInfo)
	{
		// default layout
		ZeroMemory(&layoutInfo, sizeof(LayoutInfo));
		layoutInfo.x = UNASSIGNED;
		layoutInfo.y = UNASSIGNED;
		layoutInfo.width = UNASSIGNED;
		layoutInfo.height = UNASSIGNED;
		layoutInfo.margin = 4;
		layoutInfo.marginLeft = UNASSIGNED;
		layoutInfo.marginRight = UNASSIGNED;
		layoutInfo.marginTop = UNASSIGNED;
		layoutInfo.marginBottom = UNASSIGNED;
		layoutInfo.border = 0;
		layoutInfo.borderLeft = UNASSIGNED;
		layoutInfo.borderRight = UNASSIGNED;
		layoutInfo.borderTop = UNASSIGNED;
		layoutInfo.borderBottom = UNASSIGNED;
		layoutInfo.padding = 4;
		layoutInfo.paddingLeft = UNASSIGNED;
		layoutInfo.paddingRight = UNASSIGNED;
		layoutInfo.paddingTop = UNASSIGNED;
		layoutInfo.paddingBottom = UNASSIGNED;
		layoutInfo.display = true;
		layoutInfo.visible = true;
		layoutInfo.align = LEFT;
		layoutInfo.align = TOP;
	}

	void FrameTool::SetDefaults(FrameStyle &frameStyle)
	{
		// default style
		ZeroMemory(&frameStyle, sizeof(FrameStyle));
	}

	void FrameTool::GetMetrics(LayoutInfo &layoutInfo, int &x, int &y, int &w, int &h)
	{
		x = ISASSIGNED(layoutInfo.x) ? layoutInfo.x : layoutInfo.rect.left;
		y = ISASSIGNED(layoutInfo.y) ? layoutInfo.y : layoutInfo.rect.top;
		w = ISASSIGNED(layoutInfo.width) ? layoutInfo.width : layoutInfo.rect.Width();
		h = ISASSIGNED(layoutInfo.height) ? layoutInfo.height : layoutInfo.rect.Height();
	}

	void FrameTool::GetMargins(LayoutInfo &layoutInfo, int &l, int &t, int &r, int &b)
	{
		int mg = ISASSIGNED(layoutInfo.margin) ? layoutInfo.margin : 0;
		l = ISASSIGNED(layoutInfo.marginLeft) ? layoutInfo.marginLeft : mg;
		r = ISASSIGNED(layoutInfo.marginRight) ? layoutInfo.marginRight : mg;
		t = ISASSIGNED(layoutInfo.marginTop) ? layoutInfo.marginTop : mg;
		b = ISASSIGNED(layoutInfo.marginBottom) ? layoutInfo.marginBottom : mg;
	}

	void FrameTool::GetBorders(LayoutInfo &layoutInfo, int &l, int &t, int &r, int &b)
	{
		int bd = ISASSIGNED(layoutInfo.border) ? layoutInfo.border : 0;
		l = ISASSIGNED(layoutInfo.borderLeft) ? layoutInfo.borderLeft : bd;
		r = ISASSIGNED(layoutInfo.borderRight) ? layoutInfo.borderRight : bd;
		t = ISASSIGNED(layoutInfo.borderTop) ? layoutInfo.borderTop : bd;
		b = ISASSIGNED(layoutInfo.borderBottom) ? layoutInfo.borderBottom : bd;
	}

	void FrameTool::GetContentOffsets(LayoutInfo &layoutInfo, int &l, int &t, int &r, int &b)
	{
		// margins
		int mg = ISASSIGNED(layoutInfo.margin) ? layoutInfo.margin : 0;
		int mgl = ISASSIGNED(layoutInfo.marginLeft) ? layoutInfo.marginLeft : mg;
		int mgr = ISASSIGNED(layoutInfo.marginRight) ? layoutInfo.marginRight : mg;
		int mgt = ISASSIGNED(layoutInfo.marginTop) ? layoutInfo.marginTop : mg;
		int mgb = ISASSIGNED(layoutInfo.marginBottom) ? layoutInfo.marginBottom : mg;

		// borders
		int bd = ISASSIGNED(layoutInfo.border) ? layoutInfo.border : 0;
		int bdl = ISASSIGNED(layoutInfo.borderLeft) ? layoutInfo.borderLeft : bd;
		int bdr = ISASSIGNED(layoutInfo.borderRight) ? layoutInfo.borderRight : bd;
		int bdt = ISASSIGNED(layoutInfo.borderTop) ? layoutInfo.borderTop : bd;
		int bdb = ISASSIGNED(layoutInfo.borderBottom) ? layoutInfo.borderBottom : bd;

		// padding
		int pd = ISASSIGNED(layoutInfo.padding) ? layoutInfo.padding : 0;
		int pdl = ISASSIGNED(layoutInfo.paddingLeft) ? layoutInfo.paddingLeft : pd;
		int pdr = ISASSIGNED(layoutInfo.paddingRight) ? layoutInfo.paddingRight : pd;
		int pdt = ISASSIGNED(layoutInfo.paddingTop) ? layoutInfo.paddingTop : pd;
		int pdb = ISASSIGNED(layoutInfo.paddingBottom) ? layoutInfo.paddingBottom : pd;

		l = mgl + bdl + pdl;
		t = mgt + bdt + pdt;
		r = mgr + bdr + pdr;
		b = mgb + bdb + pdb;
	}

	// Frame
	Frame::Frame() 
	{
		parentFrame = 0;
		FrameTool::SetDefaults(layoutInfo);
	}

	Frame::~Frame()
	{
	}

	bool Frame::SetParent(Frame* parent)
	{
		parentFrame = (Frame*)parent;
		return true;
	}

	Frame* Frame::GetParent()
	{
		return parentFrame;
	}

	Frame* Frame::GetRoot()
	{
		if (parentFrame)
		{
			return parentFrame->GetRoot();
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
		/*
		FrameList::iterator it = frames.begin();
		while(it != frames.end()) {
		if (*it == pFrame) {
		frames.erase(it);
		pFrame->SetParent(0);
		break;
		}
		it++;
		}
		*/
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

	bool Frame::Layout()
	{
		printf("layout %s\n", GetName());
		return true;
	}

	bool Frame::OnEvent(int eventId, void *)
	{
		return true;
	}

	bool Frame::GetRect(Rect *pRect)
	{
		(*pRect) = layoutInfo.rect;
		return true;
	}

	bool Frame::GetBorderRect(Rect *pRect)
	{
		(*pRect) = layoutInfo.rect;
		int mgl, mgt, mgr, mgb;
		FrameTool::GetMargins(layoutInfo, mgl, mgt, mgr, mgb);
		pRect->left += mgl;
		pRect->top += mgt;
		pRect->right -= mgr;
		pRect->bottom -= mgb;
		return true;
	}

	bool Frame::GetContentRect(Rect *pRect)
	{
		int ol, ot, or, ob;
		FrameTool::GetContentOffsets(layoutInfo, ol, ot, or, ob);
		pRect->left += ol;
		pRect->top += ot;
		pRect->right -= or;
		pRect->bottom -= ob;
		return true;
	}

	bool Frame::SetRect(const Rect* pRect)
	{
		layoutInfo.x = pRect->left;
		layoutInfo.y = pRect->top;
		layoutInfo.width = pRect->Width();
		layoutInfo.height = pRect->Height();
		layoutInfo.rect = (*pRect);
		return true;
	}

	void Frame::FreeFrames(Frame *frame, bool freeSelf)
	{
		while(Frame *f = frame->frames.GetFirst())
		{
			frame->frames.Remove(f);
			FreeFrames(f);
		}

		if (freeSelf)
			delete frame;
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
		printf("%s\n", GetName());

		f = frames.GetFirst();
		while(f)
		{
			f->Dump();
			f = f->next;
		}
	}
};