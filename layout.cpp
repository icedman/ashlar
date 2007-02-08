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

#include "layout.h"

namespace Ash
{
	// HFrame
	bool HFrame::Layout()
	{
		Frame::Layout();

		FrameList *frames = GetFrames();
		FrameList::iterator it;

		FrameTool lt;

		// set default values for unassigned metrics
		int xx, yy, ww, hh;
		lt.GetMetrics(layoutInfo, xx, yy, ww, hh);

		// get client offsets (margin + border + padding)
		int ol, ot, or, ob;
		lt.GetContentOffsets(layoutInfo, ol, ot, or, ob);

		// adjust to offsets
		xx += ol;
		yy += ot;
		ww -= (ol + or);
		hh -= (ot + ob);
		ww = ww > 0 ? ww : 0;
		hh = hh > 0 ? hh : 0;

		// clear calculations
		layoutInfo.totalFlex = 0;
		layoutInfo.totalChildWidths = 0;
		layoutInfo.totalChildHeights = 0;
		layoutInfo.maxChildWidth = 0;
		layoutInfo.maxChildHeight = 0;
		bool hasFlex = false;

		/*
		First pass
		- set preferred metrics for child frames
		- do precalculations for second pass
		*/
		it = frames->begin();
		while(it != frames->end())
		{
			Frame *f = *it;
			LayoutInfo *li = &f->layoutInfo;

			// get default metrics
			int fw = li->width;
			int fh = li->height;
			if (!ISASSIGNED(fh))
			{	
				// default height
				fh = 0;
				if (li->flex > 0)
				{
					// fill parent space
					fh = hh;
				} else {
					// expand height
					int ol, ot, or, ob;
					lt.GetContentOffsets(*li, ol, ot, or, ob);
					fh = li->maxChildHeight + ot + ob;
				}
			}
			if (!ISASSIGNED(fw))
			{	
				// default width
				fw = 0;
				if (li->flex == 0)
				{
					// expand width
					int ol, ot, or, ob;
					lt.GetContentOffsets(*li, ol, ot, or, ob);
					fw = li->totalChildWidths + ol + or;
				}
			}

			// set default metrics
			li->rect.Set(xx, yy, xx + fw, yy + fh);
			if (li->floating || !li->display)
			{
				// floating frame
				int fx = ISASSIGNED(li->x)? li->x : xx;
				int fy = ISASSIGNED(li->y)? li->y : yy;
				li->rect.Set(fx, fy, fx + fw, fy + fh);
				it++;
				continue;
			}

			// precalculations for second pass
			layoutInfo.totalChildWidths += fw;
			layoutInfo.totalChildHeights += fh;
			if (layoutInfo.maxChildWidth == 0 || layoutInfo.maxChildWidth < fw)
			{
				layoutInfo.maxChildWidth = fw;
			}
			if (layoutInfo.maxChildHeight == 0 || layoutInfo.maxChildHeight < fh)
			{
				layoutInfo.maxChildHeight = fh;
			}
			if (!ISASSIGNED(li->width) && li->flex > 0)
			{
				layoutInfo.totalFlex += li->flex;
				hasFlex = true;
			}
			xx += fw;
			it++;
		}

		/*
		Second pass
		- adjust flexible child frames
		- align child frames
		- flow child frames
		*/
		int flexSpace = ww - layoutInfo.totalChildWidths;
		xx = 0;
		yy = 0;
		it = frames->begin();
		while(it != frames->end())
		{
			Frame *f = *it;
			LayoutInfo *li = &f->layoutInfo;
			if (li->floating || !li->display)
			{
				f->Layout();
				it++;
				continue;
			}

			// verticalAlign
			int verticalAlignOffset = 0;
			if (layoutInfo.verticalAlign == MIDDLE)
			{
				verticalAlignOffset = (hh >> 1) - (li->rect.Height() >> 1);
			} else if (layoutInfo.verticalAlign == BOTTOM) {
				verticalAlignOffset = hh - li->rect.Height();
			}

			// align
			int alignOffset = 0;
			if (!hasFlex)
			{
				if (layoutInfo.align == CENTER)
				{
					alignOffset = (flexSpace >> 1);
				} else if (layoutInfo.align == RIGHT) {
					alignOffset = flexSpace;
				}
			}

			// adjust xy (flexible siblings causes the adjustments)
			li->rect.Translate(xx + alignOffset, yy + verticalAlignOffset);
			if (li->flex > 0 && hh == 0)
			{
				// fill parent space for expanded
				if (li->rect.Height() == 0 && !ISASSIGNED(li->height))
				{
					li->rect.bottom += layoutInfo.maxChildHeight;
				}
			}
			if (li->flex > 0 && !ISASSIGNED(li->width))
			{
				// flex item
				double pf = (double)li->flex / layoutInfo.totalFlex;
				int nw = pf * flexSpace;
				li->rect.right = li->rect.left + nw;

				// temporarily adjust rect before calling layout
				LayoutInfo tmp = f->layoutInfo;
				f->SetRect(&li->rect);
				li->width = nw;

				f->Layout();

				f->layoutInfo = tmp;
				xx += nw;
			} else {
				f->Layout();
			}
			it++;
		}

		return true;
	}

	// VFrame
	bool VFrame::Layout()
	{
		Frame::Layout();

		FrameList *frames = GetFrames();
		FrameList::iterator it;

		FrameTool lt;

		// set default values for unassigned metrics
		int xx, yy, ww, hh;
		lt.GetMetrics(layoutInfo, xx, yy, ww, hh);

		// get client offsets (margin + border + padding)
		int ol, ot, or, ob;
		lt.GetContentOffsets(layoutInfo, ol, ot, or, ob);

		// adjust to offsets
		xx += ol;
		yy += ot;
		ww -= (ol + or);
		hh -= (ot + ob);
		ww = ww > 0 ? ww : 0;
		hh = hh > 0 ? hh : 0; 

		// clear calculations
		layoutInfo.totalFlex = 0;
		layoutInfo.totalChildWidths = 0;
		layoutInfo.totalChildHeights = 0;
		layoutInfo.maxChildWidth = 0;
		layoutInfo.maxChildHeight = 0;
		bool hasFlex = false;

		/*
		First pass
		- set preferred metrics for child frames
		- do precalculations for second pass
		*/
		it = frames->begin();
		while(it != frames->end())
		{
			Frame *f = *it;
			LayoutInfo *li = &f->layoutInfo;

			// get default metrics
			int fw = li->width;
			int fh = li->height;
			if (!ISASSIGNED(fw))
			{	
				// default width
				fw = 0;
				if (li->flex > 0)
				{
					// fill parent space
					fw = ww;
				} else {
					// expand width
					int ol, ot, or, ob;
					lt.GetContentOffsets(*li, ol, ot, or, ob);
					fw = li->maxChildWidth + ol + or;
				}
			}
			if (!ISASSIGNED(fh))
			{	
				// default height
				fh = 0;
				if (li->flex == 0)
				{
					// expand height
					int ol, ot, or, ob;
					lt.GetContentOffsets(*li, ol, ot, or, ob);
					fh = li->totalChildHeights + ot + ob;
				}
			}

			// set default metrics
			li->rect.Set(xx, yy, xx + fw, yy + fh);
			if (li->floating || !li->display)
			{
				// floating frame
				int fx = ISASSIGNED(li->x)? li->x : xx;
				int fy = ISASSIGNED(li->y)? li->y : yy;
				li->rect.Set(fx, fy, fx + fw, fy + fh);
				it++;
				continue;
			}

			// precalculations for second pass
			layoutInfo.totalChildWidths += fw;
			layoutInfo.totalChildHeights += fh;
			if (layoutInfo.maxChildWidth == 0 || layoutInfo.maxChildWidth < fw)
			{
				layoutInfo.maxChildWidth = fw;
			}
			if (layoutInfo.maxChildHeight == 0 || layoutInfo.maxChildHeight < fh)
			{
				layoutInfo.maxChildHeight = fh;
			}
			if (!ISASSIGNED(li->height) && li->flex > 0)
			{
				layoutInfo.totalFlex += li->flex;
				hasFlex = true;
			}
			yy += fh;
			it++;
		}

		/*
		Second pass
		- adjust flexible child frames
		- align child frames
		- flow child frames
		*/
		int flexSpace = hh - layoutInfo.totalChildHeights;
		xx = 0;
		yy = 0;
		it = frames->begin();
		while(it != frames->end())
		{
			Frame *f = *it;
			LayoutInfo *li = &f->layoutInfo;
			if (li->floating || !li->display)
			{
				f->Layout();
				it++;
				continue;
			}

			// align
			int alignOffset = 0;
			if (layoutInfo.align == CENTER)
			{
				alignOffset = (ww >> 1) - (li->rect.Width() >> 1);
			} else if (layoutInfo.align == RIGHT) {
				alignOffset = ww - li->rect.Width();
			}

			// verticalAlign
			int verticalAlignOffset = 0;
			if (!hasFlex)
			{
				if (layoutInfo.verticalAlign == MIDDLE)
				{
					verticalAlignOffset = (flexSpace >> 1);
				} else if (layoutInfo.verticalAlign == BOTTOM) {
					verticalAlignOffset = flexSpace;
				}
			}

			// adjust xy (flexible siblings causes the adjustments)
			li->rect.Translate(xx + alignOffset, yy + verticalAlignOffset);
			if (li->flex > 0 && ww == 0)
			{
				// fill parent space for expanded
				if (li->rect.Width() == 0 && !ISASSIGNED(li->width))
				{
					li->rect.right += layoutInfo.maxChildWidth;
				}
			}
			if (li->flex > 0 && !ISASSIGNED(li->height))
			{
				// flex item
				double pf = (double)li->flex / layoutInfo.totalFlex;
				int nh = pf * flexSpace;
				li->rect.bottom = li->rect.top + nh;

				// temporarily adjust rect before calling layout
				LayoutInfo tmp = f->layoutInfo;
				f->SetRect(&li->rect);
				li->height = nh;

				f->Layout();

				f->layoutInfo = tmp;
				yy += nh;
			} else {
				f->Layout();
			}
			it++;
		}

		return true;
	}
}
