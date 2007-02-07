/*!
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

#include "render.h"

namespace Ash
{
	bool RenderEngine::Render(Frame *pFrame)
	{
		LayoutInfo *li = &pFrame->layoutInfo;
		bool draw = true;

		// skip invisible
		draw &= (li->visible && li->display);

		Rect r;
		pFrame->GetRect(&r);
		draw &= (r.Width() > 0 && r.Height() > 0);

		if (draw)
		{
			DrawRect(&r, RGB(255,0,0));
			pFrame->GetBorderRect(&r);
			DrawRect(&r, RGB(0,0,255));
			printf("render %s %d (%d, %d)-(%d, %d)\n", pFrame->GetName(), pFrame, r.left, r.top, r.right, r.bottom);
		} else {
			printf("skip render %s %d (%d, %d)-(%d, %d)\n", pFrame->GetName(), pFrame, r.left, r.top, r.right, r.bottom);
		}

		// draw children
		FrameList *frames = pFrame->GetFrames();
		FrameList::iterator it = frames->begin();
		while(it != frames->end())
		{
			Render(*it);
			it++;
		}

		return true;
	}

	void RenderEngine::DrawRect(const Rect* pRect, long color)
	{
		LOGBRUSH lb; 
		HGDIOBJ hPen, hPenOld; 

		lb.lbStyle = BS_SOLID; 
		lb.lbColor = color; 
		lb.lbHatch = 0; 

		hPen = ExtCreatePen(PS_SOLID, 1, &lb, 0, NULL); 
		hPenOld = SelectObject(hdcTarget, hPen);

		POINT p;
		MoveToEx(hdcTarget, pRect->left, pRect->top, &p);
		LineTo(hdcTarget, pRect->right, pRect->top);
		LineTo(hdcTarget, pRect->right, pRect->bottom);
		LineTo(hdcTarget, pRect->left, pRect->bottom);
		LineTo(hdcTarget, pRect->left, pRect->top);

		SelectObject(hdcTarget, hPenOld);
		DeleteObject(hPen);
	}
}