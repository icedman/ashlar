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

#include "framestyle.h"
#include "safenode.h"

namespace Layout
{
	// FrameTool
	int FrameTool::StringToAlign(DOMString *str, int defaultValue)
	{
		if (!str)
			return defaultValue;
		const char *c_str = str->c_str();
		if (strcmp(c_str, "left") == 0)
			return LEFT;
		if (strcmp(c_str, "right") == 0)
			return RIGHT;
		if (strcmp(c_str, "center") == 0)
			return CENTER;
		if (strcmp(c_str, "top") == 0)
			return TOP;
		if (strcmp(c_str, "bottom") == 0)
			return BOTTOM;
		if (strcmp(c_str, "middle") == 0)
			return MIDDLE;
		return defaultValue;
	}

	long FrameTool::StringToColor(DOMString *str, int *r, int *g, int *b, int *a)
	{
		int rr, gg, bb, aa;
		return atoi(str->c_str());
	}

	void FrameTool::SetDefaults(FrameStyle &frameStyle)
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
		frameStyle.layout.align = LEFT;
		frameStyle.layout.align = TOP;

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
	}

	void FrameTool::GetMetrics(LayoutInfo &layoutInfo, int &x, int &y, int &w, int &h)
	{
		x = ISASSIGNED(layoutInfo.x) ? layoutInfo.x : layoutInfo.rect.left;
		y = ISASSIGNED(layoutInfo.y) ? layoutInfo.y : layoutInfo.rect.top;
		w = ISASSIGNED(layoutInfo.width) ? layoutInfo.width : layoutInfo.rect.Width();
		h = ISASSIGNED(layoutInfo.height) ? layoutInfo.height : layoutInfo.rect.Height();
	}

	void FrameTool::GetBorders(Borders &borders, int &l, int &t, int &r, int &b)
	{
		int bd = ISASSIGNED(borders.width) ? borders.width : 0;
		l = ISASSIGNED(borders.left) ? borders.left : bd;
		r = ISASSIGNED(borders.right) ? borders.right : bd;
		t = ISASSIGNED(borders.top) ? borders.top : bd;
		b = ISASSIGNED(borders.bottom) ? borders.bottom : bd;
	}

	void FrameTool::GetContentOffsets(FrameStyle &frameStyle, int &l, int &t, int &r, int &b)
	{
		// margins
		int mgl, mgt, mgr, mgb;
		GetBorders(frameStyle.margin, mgl, mgt, mgr, mgb);

		// borders
		int bdl, bdt, bdr, bdb;
		GetBorders(frameStyle.border, bdl, bdr, bdt, bdb);

		// padding
		int pdl, pdt, pdr, pdb;
		GetBorders(frameStyle.padding, pdl, pdr, pdt, pdb);

		l = mgl + bdl + pdl;
		t = mgt + bdt + pdt;
		r = mgr + bdr + pdr;
		b = mgb + bdb + pdb;
	}

	void FrameTool::SetStyleFromXml(FrameStyle &fs, DOMNode *el)
	{
		SetLayoutFromXml(fs.layout, el);
	}

	void FrameTool::SetLayoutFromXml(LayoutInfo &li, DOMNode *el)
	{
		SafeNode snode((Element*)el);
		li.x = snode.GetAttribute(&DOMString("x"))->ValueInt(li.y);
		li.y = snode.GetAttribute(&DOMString("y"))->ValueInt(li.y);
		li.width = snode.GetAttribute(&DOMString("width"))->ValueInt(li.width);
		li.height = snode.GetAttribute(&DOMString("height"))->ValueInt(li.height);
		li.flex = snode.GetAttribute(&DOMString("flex"))->ValueInt(li.flex);

		SafeNode *dxLayout = snode.GetElement(&DOMString("style"))->GetElement(&DOMString("layout"));
		li.x = dxLayout->GetAttribute(&DOMString("x"))->ValueInt(li.y);
		li.y = dxLayout->GetAttribute(&DOMString("y"))->ValueInt(li.y);
		li.width = dxLayout->GetAttribute(&DOMString("width"))->ValueInt(li.width);
		li.height = dxLayout->GetAttribute(&DOMString("height"))->ValueInt(li.height);
		li.flex = dxLayout->GetAttribute(&DOMString("flex"))->ValueInt(li.flex);
	}
}