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

#include "styledom.h"

namespace Layout
{
	/*
	Todo:
	- add default values everywhere
	- add min/max checking
	*/

	int StringToAlign(DOMString *str, int defaultValue)
	{
		if (!str)
			return defaultValue;
		if (*str == DOMString("left"))
			return LEFT;
		if (*str == DOMString("right"))
			return RIGHT;
		if (*str == DOMString("center"))
			return CENTER;
		if (*str == DOMString("top"))
			return TOP;
		if (*str == DOMString("bottom"))
			return BOTTOM;
		if (*str == DOMString("middle"))
			return MIDDLE;
		return defaultValue;
	}

	int StringToStyle(DOMString *str, int defaultValue)
	{
		// gradient
		if (!str)
			return defaultValue;
		if (*str == DOMString("linear"))
			return LINEAR;
		if (*str == DOMString("radial"))
			return RADIAL;
		// background
		if (*str == DOMString("fill"))
			return FILL;
		if (*str == DOMString("image"))
			return IMAGE;
		return defaultValue;
	}

	long StringToColor(DOMString *str, long defaultValue)
	{
		if (!str)
			return defaultValue;

		const char *c = str->c_str();
		if (c[0] == '#')
			c++;

		DOMString tmp(c);
		DOMString rgb(tmp, 0, 6);
		DOMString alpha(tmp, 6);
		long clr = strtol(rgb.c_str(), (char**)rgb.c_str() + 6, 16);
		int rr = GetRValue(clr);
		int gg = GetGValue(clr);
		int bb = GetBValue(clr);

		// switch rr & bb
		return RGB(bb, gg, rr);
	}

	void GetStyleXml(FrameStyle &fs, DOMNode *el)
	{
		GetLayoutXml(fs.layout, el);
		SafeNode snode((Element*)el);
		SafeNode *sstyle = snode.GetElement(&DOMString("style"));
		GetLayoutXml(fs.layout, sstyle->GetElement(&DOMString("layout"))->Node());
		GetGradientXml(fs.gradient, sstyle->GetElement(&DOMString("gradient"))->Node());
		GetBordersXml(fs.margin, sstyle->GetElement(&DOMString("margin"))->Node());
		GetBordersXml(fs.border, sstyle->GetElement(&DOMString("border"))->Node());
		GetBordersXml(fs.padding, sstyle->GetElement(&DOMString("padding"))->Node());
		GetBorderStyleXml(fs.borderStyle, sstyle->GetElement(&DOMString("border"))->Node());
	}

	void GetBorderStyleXml(BorderStyle &bs, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode snode((Element*)el);
		//bs.style = snode.GetValue(&DOMString("style"))->ValueInt(bs.style);
		bs.color = StringToColor(snode.GetValue(&DOMString("color"))->Value(), 0);
		//bs.imageId = snode.GetValue(&DOMString("imageId"))->ValueInt(bs.imageId);
		GetBordersXml(bs.radius, snode.GetElement(&DOMString("radius"))->Node());
		if (snode.GetElement(&DOMString("radius"))->Node())
		//bs.bevelStyle = snode.GetValue(&DOMString("bevelStyle"))->ValueInt(bs.bevelStyle);
		bs.bevelColor = StringToColor(snode.GetValue(&DOMString("bevelColor"))->Value(), 0);
		bs.bevelWidth = snode.GetValue(&DOMString("bevelWidth"))->ValueInt(bs.bevelWidth);
	}

	void GetLayoutXml(LayoutInfo &li, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode snode((Element*)el);
		li.x = snode.GetValue(&DOMString("x"))->ValueInt(li.y);
		li.y = snode.GetValue(&DOMString("y"))->ValueInt(li.y);
		li.width = snode.GetValue(&DOMString("width"))->ValueInt(li.width);
		li.height = snode.GetValue(&DOMString("height"))->ValueInt(li.height);
		li.flex = snode.GetValue(&DOMString("flex"))->ValueInt(li.flex);
		li.align = StringToAlign(snode.GetValue(&DOMString("align"))->Value(), LEFT);
		li.verticalAlign = StringToAlign(snode.GetValue(&DOMString("valign"))->Value(), TOP);
	}

	void GetBordersXml(Borders &br, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode snode((Element*)el);
		br.width = snode.ValueInt(br.width);
		br.width = snode.GetValue(&DOMString("width"))->ValueInt(br.width);
		br.left = snode.GetValue(&DOMString("left"))->ValueInt(br.left);
		br.right = snode.GetValue(&DOMString("right"))->ValueInt(br.right);
		br.top = snode.GetValue(&DOMString("top"))->ValueInt(br.top);
		br.bottom = snode.GetValue(&DOMString("bottom"))->ValueInt(br.bottom);

		// radius
		br.left = snode.GetValue(&DOMString("leftTop"))->ValueInt(br.left);
		br.top = snode.GetValue(&DOMString("rightTop"))->ValueInt(br.top);
		br.right = snode.GetValue(&DOMString("rightBottom"))->ValueInt(br.right);
		br.bottom = snode.GetValue(&DOMString("leftBottom"))->ValueInt(br.bottom);
	}

	void GetGradientXml(Gradient &gr, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode sgradient((Element*)el);
		gr.style = StringToStyle(sgradient.GetValue(&DOMString("type"))->Value(), 0);
		if (!gr.style)
			return;

		// don't bother if no gradient style
		gr.x = sgradient.GetValue(&DOMString("x"))->ValueInt(gr.x);
		gr.y = sgradient.GetValue(&DOMString("y"))->ValueInt(gr.y);
		gr.radius = sgradient.GetValue(&DOMString("radius"))->ValueInt(gr.radius);
		gr.x2 = sgradient.GetValue(&DOMString("x2"))->ValueInt(gr.x2);
		gr.y2 = sgradient.GetValue(&DOMString("y2"))->ValueInt(gr.y2);
		gr.radius2 = sgradient.GetValue(&DOMString("radius2"))->ValueInt(gr.radius2);

		SafeNode *scolors = sgradient.GetElement(&DOMString("color"));

		// colors
		gr.colorCount = 0;
		for(int i = 0; i<scolors->Length(); i++)
		{
			if (scolors->Item(i)->Value())
			{	
				gr.colors[gr.colorCount] = StringToColor(scolors->Item(i)->Value(), gr.colors[gr.colorCount]);
				gr.colorCount++;
				if (gr.colorCount == GRADIENT_COLOR_LIMIT)
					break;
			}
		}

		// color offsets
		for(int i = 0, j = 0; i<scolors->Length(); i++)
		{
			if (scolors->Item(i)->Value())
			{	
				gr.offsets[j] = 255 * ((double)j+1/gr.colorCount);
				gr.offsets[j] = scolors->Item(i)->GetValue(&DOMString("offset"))->ValueInt(gr.offsets[j]);
				j++;
			}
		}
	}
}