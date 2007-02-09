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
#include "framestyle.h"

namespace Layout
{
	// Frame
	Frame::Frame() 
	{
		parentFrame = 0;
		FrameTool::SetDefaults(frameStyle);
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
		(*pRect) = frameStyle.layout.rect;
		return true;
	}

	bool Frame::GetBorderRect(Rect *pRect)
	{
		(*pRect) = frameStyle.layout.rect;
		int mgl, mgt, mgr, mgb;
		FrameTool::GetBorders(frameStyle.margin, mgl, mgt, mgr, mgb);
		pRect->left += mgl;
		pRect->top += mgt;
		pRect->right -= mgr;
		pRect->bottom -= mgb;
		return true;
	}

	bool Frame::GetContentRect(Rect *pRect)
	{
		int ol, ot, or, ob;
		FrameTool::GetContentOffsets(frameStyle, ol, ot, or, ob);
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