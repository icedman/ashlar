#include "layout.h"

namespace Ash
{
	// HFrame
	bool HFrame::Layout()
	{
		return true;
	}

	// VFrame
	bool VFrame::Layout()
	{
		Frame::Layout();

		/*
		Todo:
		- borderWidth
		- refactor
		*/

		FrameList *frames = GetFrames();
		FrameList::iterator it;

		// set default values for unassigned metrics
		int xx = ISASSIGNED(layoutInfo.x) ? layoutInfo.x : layoutInfo.rect.left;
		int yy = ISASSIGNED(layoutInfo.y) ? layoutInfo.y : layoutInfo.rect.top;
		int ww = ISASSIGNED(layoutInfo.width) ? layoutInfo.width : layoutInfo.rect.Width();
		int hh = ISASSIGNED(layoutInfo.height) ? layoutInfo.height : layoutInfo.rect.Height();

		// margins
		int mg = ISASSIGNED(layoutInfo.margin) ? layoutInfo.margin : 0;
		int mgl = ISASSIGNED(layoutInfo.marginLeft) ? layoutInfo.marginLeft : mg;
		int mgr = ISASSIGNED(layoutInfo.marginRight) ? layoutInfo.marginRight : mg;
		int mgt = ISASSIGNED(layoutInfo.marginTop) ? layoutInfo.marginTop : mg;
		int mgb = ISASSIGNED(layoutInfo.marginBottom) ? layoutInfo.marginBottom : mg;

		// borders

		// padding
		int pd = ISASSIGNED(layoutInfo.padding) ? layoutInfo.padding : 0;
		int pdl = ISASSIGNED(layoutInfo.paddingLeft) ? layoutInfo.paddingLeft : pd;
		int pdr = ISASSIGNED(layoutInfo.paddingRight) ? layoutInfo.paddingRight : pd;
		int pdt = ISASSIGNED(layoutInfo.paddingTop) ? layoutInfo.paddingTop : pd;
		int pdb = ISASSIGNED(layoutInfo.paddingBottom) ? layoutInfo.paddingBottom : pd;

		xx += mgl + pdl;
		yy += mgt + pdt;
		ww -= (mgl + mgr + pdl + pdr);
		hh -= (mgt + mgb + pdt + pdb);
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
			int fw = ISASSIGNED(li->width) ? li->width : UNASSIGNED;
			int fh = ISASSIGNED(li->height) ? li->height : UNASSIGNED;
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
					int fmg = ISASSIGNED(li->margin) ? li->margin : 0;
					int fmgl = ISASSIGNED(li->marginLeft) ? li->marginLeft : fmg;
					int fmgr = ISASSIGNED(li->marginRight) ? li->marginRight : fmg;
					int fpd = ISASSIGNED(li->padding) ? li->padding : 0;
					int fpdl = ISASSIGNED(li->paddingLeft) ? li->paddingLeft : fpd;
					int fpdr = ISASSIGNED(li->paddingRight) ? li->paddingRight : fpd;
					fw = li->maxChildWidth + fmgl + fmgr + fpdl + fpdr;
				}
			}
			if (!ISASSIGNED(fh))
			{	
				// default height
				fh = 0;
				if (li->flex == 0)
				{
					// expand height
					int fmg = ISASSIGNED(li->margin) ? li->margin : 0;
					int fmgt = ISASSIGNED(li->marginTop) ? li->marginTop : fmg;
					int fmgb = ISASSIGNED(li->marginBottom) ? li->marginBottom : fmg;
					int fpd = ISASSIGNED(li->padding) ? li->padding : 0;
					int fpdt = ISASSIGNED(li->paddingTop) ? li->paddingTop : fpd;
					int fpdb = ISASSIGNED(li->paddingBottom) ? li->paddingBottom : fpd;
					fh = li->totalChildHeights + fmgt + fmgb + fpdt + fpdb;
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
				//li->x = fx;
				//li->y = fy;
				it++;
				continue;
			}
			//li->x = xx;
			//li->y = yy;

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
			if (layoutInfo.align == Align::CENTER)
			{
				alignOffset = (ww >> 1) - (li->rect.Width() >> 1);
			} else if (layoutInfo.align == Align::RIGHT) {
				alignOffset = ww - li->rect.Width();
			}
			// verticalAlign
			int verticalAlignOffset = 0;
			if (!hasFlex)
			{
				if (layoutInfo.verticalAlign == Align::MIDDLE)
				{
					verticalAlignOffset = (flexSpace >> 1);
				} else if (layoutInfo.verticalAlign == Align::BOTTOM) {
					verticalAlignOffset = flexSpace;
				}
			}

			// adjust xy
			li->rect.Translate(xx + alignOffset, yy + verticalAlignOffset);
			//li->x += (xx + alignOffset);
			//li->y += (yy + verticalAlignOffset);
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
