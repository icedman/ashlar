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

#include <layout/layout.h>
#include <layout/framestyle.h>

namespace Layout
{
	// HFrame
	bool HFrame::Layout()
	{
		Frame::Layout();

		FrameList *frames = GetFrames();

		// set default values for unassigned metrics
		int xx, yy, ww, hh;
		GetMetrics(frameStyle.layout, xx, yy, ww, hh);

		// get client offsets (margin + border + padding)
		int ol, ot, or, ob;
		GetContentOffsets(frameStyle, ol, ot, or, ob);

		// adjust to offsets
		xx += ol;
		yy += ot;
		ww -= (ol + or);
		hh -= (ot + ob);
		ww = ww > 0 ? ww : 0;
		hh = hh > 0 ? hh : 0;

		// clear calculations
		frameStyle.layout.totalFlex = 0;
		frameStyle.layout.totalChildWidths = 0;
		frameStyle.layout.totalChildHeights = 0;
		frameStyle.layout.maxChildWidth = 0;
		frameStyle.layout.maxChildHeight = 0;
		bool hasFlex = false;

		/*
		First pass
		- set preferred metrics for child frames
		- do precalculations for second pass
		*/
		Frame *f = frames->GetFirst();
		while(f)
		{
			LayoutInfo *li = &f->frameStyle.layout;
			FrameStyle *fs = &f->frameStyle;

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
					GetContentOffsets(*fs, ol, ot, or, ob);
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
					GetContentOffsets(*fs, ol, ot, or, ob);
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
				f = f->next;
				continue;
			}

			// precalculations for second pass
			frameStyle.layout.totalChildWidths += fw;
			frameStyle.layout.totalChildHeights += fh;
			if (frameStyle.layout.maxChildWidth == 0 || frameStyle.layout.maxChildWidth < fw)
			{
				frameStyle.layout.maxChildWidth = fw;
			}
			if (frameStyle.layout.maxChildHeight == 0 || frameStyle.layout.maxChildHeight < fh)
			{
				frameStyle.layout.maxChildHeight = fh;
			}
			if (!ISASSIGNED(li->width) && li->flex > 0)
			{
				frameStyle.layout.totalFlex += li->flex;
				hasFlex = true;
			}
			xx += fw;
			f = f->next;
		}

		/*
		Second pass
		- adjust flexible child frames
		- align child frames
		- flow child frames
		*/
		int flexSpace = ww - frameStyle.layout.totalChildWidths;
		xx = 0;
		yy = 0;
		f = frames->GetFirst();
		while(f)
		{
			LayoutInfo *li = &f->frameStyle.layout;
			if (li->floating || !li->display)
			{
				f->Layout();
				f = f->next;
				continue;
			}

			// verticalAlign
			int verticalAlignOffset = 0;
			if (frameStyle.layout.verticalAlign == ALIGN_MIDDLE)
			{
				verticalAlignOffset = (hh >> 1) - (li->rect.Height() >> 1);
			} else if (frameStyle.layout.verticalAlign == ALIGN_BOTTOM) {
				verticalAlignOffset = hh - li->rect.Height();
			}

			// align
			int alignOffset = 0;
			if (!hasFlex)
			{
				if (frameStyle.layout.align == ALIGN_CENTER)
				{
					alignOffset = (flexSpace >> 1);
				} else if (frameStyle.layout.align == ALIGN_RIGHT) {
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
					li->rect.bottom += frameStyle.layout.maxChildHeight;
				}
			}
			if (li->flex > 0 && !ISASSIGNED(li->width))
			{
				// flex item
				double pf = (double)li->flex / frameStyle.layout.totalFlex;
				int nw = pf * flexSpace;
				li->rect.right = li->rect.left + nw;

				// temporarily adjust rect before calling layout
				LayoutInfo tmp = f->frameStyle.layout;
				f->SetRect(&li->rect);
				li->width = nw;

				f->Layout();

				f->frameStyle.layout = tmp;
				xx += nw;
			} else {
				f->Layout();
			}
			f = f->next;
		}

		return true;
	}

	// VFrame
	bool VFrame::Layout()
	{
		Frame::Layout();

		FrameList *frames = GetFrames();

		// set default values for unassigned metrics
		int xx, yy, ww, hh;
		GetMetrics(frameStyle.layout, xx, yy, ww, hh);
		// get client offsets (margin + border + padding)
		int ol, ot, or, ob;
		GetContentOffsets(frameStyle, ol, ot, or, ob);

		// adjust to offsets
		xx += ol;
		yy += ot;
		ww -= (ol + or);
		hh -= (ot + ob);
		ww = ww > 0 ? ww : 0;
		hh = hh > 0 ? hh : 0; 

		// clear calculations
		frameStyle.layout.totalFlex = 0;
		frameStyle.layout.totalChildWidths = 0;
		frameStyle.layout.totalChildHeights = 0;
		frameStyle.layout.maxChildWidth = 0;
		frameStyle.layout.maxChildHeight = 0;
		bool hasFlex = false;

		/*
		First pass
		- set preferred metrics for child frames
		- do precalculations for second pass
		*/
		Frame *f = frames->GetFirst();
		while(f)
		{
			LayoutInfo *li = &f->frameStyle.layout;
			FrameStyle *fs = &f->frameStyle;

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
					GetContentOffsets(*fs, ol, ot, or, ob);
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
					GetContentOffsets(*fs, ol, ot, or, ob);
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
				f = f->next;
				continue;
			}

			// precalculations for second pass
			frameStyle.layout.totalChildWidths += fw;
			frameStyle.layout.totalChildHeights += fh;
			if (frameStyle.layout.maxChildWidth == 0 || frameStyle.layout.maxChildWidth < fw)
			{
				frameStyle.layout.maxChildWidth = fw;
			}
			if (frameStyle.layout.maxChildHeight == 0 || frameStyle.layout.maxChildHeight < fh)
			{
				frameStyle.layout.maxChildHeight = fh;
			}
			if (!ISASSIGNED(li->height) && li->flex > 0)
			{
				frameStyle.layout.totalFlex += li->flex;
				hasFlex = true;
			}
			yy += fh;
			f = f->next;
		}

		/*
		Second pass
		- adjust flexible child frames
		- align child frames
		- flow child frames
		*/
		int flexSpace = hh - frameStyle.layout.totalChildHeights;
		xx = 0;
		yy = 0;
		f = frames->GetFirst();
		while(f)
		{
			LayoutInfo *li = &f->frameStyle.layout;
			if (li->floating || !li->display)
			{
				f->Layout();
				f = f->next;
				continue;
			}

			// align
			int alignOffset = 0;
			if (frameStyle.layout.align == ALIGN_CENTER)
			{
				alignOffset = (ww >> 1) - (li->rect.Width() >> 1);
			} else if (frameStyle.layout.align == ALIGN_RIGHT) {
				alignOffset = ww - li->rect.Width();
			}

			// verticalAlign
			int verticalAlignOffset = 0;
			if (!hasFlex)
			{
				if (frameStyle.layout.verticalAlign == ALIGN_MIDDLE)
				{
					verticalAlignOffset = (flexSpace >> 1);
				} else if (frameStyle.layout.verticalAlign == ALIGN_BOTTOM) {
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
					li->rect.right += frameStyle.layout.maxChildWidth;
				}
			}
			if (li->flex > 0 && !ISASSIGNED(li->height))
			{
				// flex item
				double pf = (double)li->flex / frameStyle.layout.totalFlex;
				int nh = pf * flexSpace;
				li->rect.bottom = li->rect.top + nh;

				// temporarily adjust rect before calling layout
				LayoutInfo tmp = f->frameStyle.layout;
				f->SetRect(&li->rect);
				li->height = nh;

				f->Layout();

				f->frameStyle.layout = tmp;
				yy += nh;
			} else {
				f->Layout();
			}
			f = f->next;
		}

		return true;
	}
}
