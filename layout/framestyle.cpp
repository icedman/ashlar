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

#include <layout/framestyle.h>

namespace Layout
{

	void SetStyleDefaults(FrameStyle &frameStyle)
	{
		// default style
		ZeroMemory(&frameStyle, sizeof(FrameStyle));

		// layout
		frameStyle.layout.x = UNASSIGNED;
		frameStyle.layout.y = UNASSIGNED;
		frameStyle.layout.width = UNASSIGNED;
		frameStyle.layout.height = UNASSIGNED;
		frameStyle.layout.display = true;
		frameStyle.layout.visible = true;
		frameStyle.layout.align = ALIGN_LEFT;
		frameStyle.layout.align = ALIGN_TOP;

		// margins
		frameStyle.margin.width = 4;
		frameStyle.margin.left = UNASSIGNED;
		frameStyle.margin.right = UNASSIGNED;
		frameStyle.margin.top = UNASSIGNED;
		frameStyle.margin.bottom = UNASSIGNED;

		// borders
		frameStyle.border.width = 0;
		frameStyle.border.left = UNASSIGNED;
		frameStyle.border.right = UNASSIGNED;
		frameStyle.border.top = UNASSIGNED;
		frameStyle.border.bottom = UNASSIGNED;

		// padding
		frameStyle.padding.width = 4;
		frameStyle.padding.left = UNASSIGNED;
		frameStyle.padding.right = UNASSIGNED;
		frameStyle.padding.top = UNASSIGNED;
		frameStyle.padding.bottom = UNASSIGNED;

		// borderStyle
		frameStyle.borderStyle.radius.width = 0;
		frameStyle.borderStyle.radius.left = UNASSIGNED;
		frameStyle.borderStyle.radius.right = UNASSIGNED;
		frameStyle.borderStyle.radius.top = UNASSIGNED;
		frameStyle.borderStyle.radius.bottom = UNASSIGNED;
	}

	void GetMetrics(LayoutInfo &layoutInfo, int &x, int &y, int &w, int &h)
	{
		x = ISASSIGNED(layoutInfo.x) ? layoutInfo.x : layoutInfo.rect.left;
		y = ISASSIGNED(layoutInfo.y) ? layoutInfo.y : layoutInfo.rect.top;
		w = ISASSIGNED(layoutInfo.width) ? layoutInfo.width : layoutInfo.rect.Width();
		h = ISASSIGNED(layoutInfo.height) ? layoutInfo.height : layoutInfo.rect.Height();
	}

	void GetBorders(Borders &borders, int &l, int &t, int &r, int &b)
	{
		int bd = ISASSIGNED(borders.width) ? borders.width : 0;
		l = ISASSIGNED(borders.left) ? borders.left : bd;
		t = ISASSIGNED(borders.top) ? borders.top : bd;
		r = ISASSIGNED(borders.right) ? borders.right : bd;
		b = ISASSIGNED(borders.bottom) ? borders.bottom : bd;
	}

	void GetContentOffsets(FrameStyle &frameStyle, int &l, int &t, int &r, int &b)
	{
		// margins
		int mgl, mgt, mgr, mgb;
		GetBorders(frameStyle.margin, mgl, mgt, mgr, mgb);

		// borders
		int bdl, bdt, bdr, bdb;
		GetBorders(frameStyle.border, bdl, bdt, bdr, bdb);

		// padding
		int pdl, pdt, pdr, pdb;
		GetBorders(frameStyle.padding, pdl, pdt, pdr, pdb);

		l = mgl + bdl + pdl;
		t = mgt + bdt + pdt;
		r = mgr + bdr + pdr;
		b = mgb + bdb + pdb;
	}

	int GetMaxBorder(Borders &bs)
	{
		int max = ISASSIGNED(bs.width) ? bs.width : 0;
		max = bs.left > max ? bs.left : max;
		max = bs.right > max ? bs.right : max;
		max = bs.top > max ? bs.top : max;
		max = bs.bottom > max ? bs.bottom : max;
		return max;
	}

}