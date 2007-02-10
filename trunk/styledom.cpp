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

	void SetStyleFromXml(FrameStyle &fs, DOMNode *el)
	{
		SetLayoutFromXml(fs.layout, el);
		SafeNode snode((Element*)el);
		SafeNode *sstyle = snode.GetElement(&DOMString("style"));
		SetLayoutFromXml(fs.layout, sstyle->GetElement(&DOMString("layout"))->Node());
		SetGradientFromXml(fs.gradient, sstyle->GetElement(&DOMString("gradient"))->Node());
	}

	void SetLayoutFromXml(LayoutInfo &li, DOMNode *el)
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

	void SetGradientFromXml(Gradient &gr, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode sgradient((Element*)el);
		gr.style = StringToStyle(sgradient.GetValue(&DOMString("style"))->Value(), 0);
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