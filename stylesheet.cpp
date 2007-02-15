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

#include "stylesheet.h"
#include "safenode.h"
#include "frames.h"

using namespace Layout;

namespace Dom
{

	inline void ObjectToId(DOMString &s, void *p)
	{
		char sid[32] = "";
		sprintf(sid, "&%d", p);
		s = sid;
	}

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

	void GetBorderStyleXml(BorderStyle &bs, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode snode((Element*)el);
		bs.color = StringToColor(snode.GetValue(&DOMString("color"))->Value(), bs.color);
		GetBordersXml(bs.radius, snode.GetElement(&DOMString("radius"))->Node());
	}

	void GetLayoutXml(LayoutInfo &li, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode snode((Element*)el);
		li.x = snode.GetValue(&DOMString("x"))->ValueInt(li.x);
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

	// StyleSheet
	bool StyleSheet::Load(Element *element)
	{
		NodeList *nl = element->GetElementsByTagName(&DOMString("style"));

		DOMNode *n = nl->GetFirst();
		while(n)
		{
			AddStyle((Element*)n);
			n = n->NextSibling();
		}

		delete nl;
		return true;
	}

	bool StyleSheet::AddStyle(Element *style)
	{
		if (!style->ParentNode())
			return false;

		Element *e = (Element*)AppendChild(style->Clone(true));
		DOMString selector;
		DOMString id;
		DOMString className;
		DOMString pseudoClass;

		GetStyleName(e, selector, id, className, pseudoClass);

		// self embedded style
		if (style->ParentNode()->nodeName != "stylesheet")
		{
			if (id == "")
			{
				ObjectToId(id, style->ParentNode());
			}
		}

		e->SetAttribute(&DOMString("selector"), &selector);
		e->SetAttribute(&DOMString("id"), &id);
		e->SetAttribute(&DOMString("className"), &className);
		e->SetAttribute(&DOMString("pseudoClass"), &pseudoClass);
		return true;
	}

	bool StyleSheet::GetStyleName(Element *e, DOMString &selector, DOMString &className, DOMString &id, DOMString &pseudoClass)
	{
		selector = "";
		id = "";
		className = "";
		pseudoClass = "";

		SafeNode snode(e);

		DOMString *name = snode.GetValue(&DOMString("name"))->Value();
		if (!name)
			return false;

		int clIdx = name->find_first_of(".");
		int psIdx = name->find_first_of(":");
		int idIdx = name->find_first_of("#");

		int any = name->find_first_of(":.#");
		any = (any!=-1) ? any : name->size();
		selector = DOMString(*name, 0, any);
		if (clIdx!=-1)
		{
			DOMString tmp(*name, clIdx + 1);
			any = tmp.find_first_of(":.#");
			any = (any!=-1) ? any : tmp.size();
			className = DOMString(tmp, 0, any);
		}
		if (idIdx!=-1)
		{
			DOMString tmp(*name, idIdx + 1);
			any = tmp.find_first_of(":.#");
			any = (any!=-1) ? any : tmp.size();
			id = DOMString(tmp, 0, any);
		}
		if (psIdx!=-1)
		{
			DOMString tmp(*name, psIdx + 1);
			any = tmp.find_first_of(":.#");
			any = (any!=-1) ? any : tmp.size();
			pseudoClass = DOMString(tmp, 0, any);
		}

		return true;
	}

	bool StyleSheet::GetSelector(Element *e, DOMString &selector, DOMString &className, DOMString &id, DOMString &pseudoClass)
	{
		selector = "";
		id = "";
		className = "";
		pseudoClass = "";

		SafeNode snode(e);
		DOMString *tmp;

		// selector
		selector = *e->TagName();

		// id
		tmp = snode.GetAttribute(&DOMString("id"))->Value();
		if (tmp)
		{
			id = *tmp;
		}

		// class
		tmp = snode.GetAttribute(&DOMString("class"))->Value();
		if (tmp)
		{
			className = *tmp;
		}

		// pseudo class
		tmp = snode.GetAttribute(&DOMString("#pseudoClass"))->Value();
		if (tmp)
		{
			pseudoClass = *tmp;
		}

		return true;
	}

	bool StyleSheet::ApplyStyle(Element *element)
	{
		Frame *f = (Frame*)element->GetData();
		DOMNode *n;
		if (f)
		{
			DOMNode *n = FirstChild();
			DOMString empty = "";

			DOMString elSelector;
			DOMString elId;
			DOMString elClassName;
			DOMString elPseudoClass;
			GetSelector(element, elSelector, elId, elClassName, elPseudoClass);
			SafeNode sel(element);


			if (elId == "")
			{
				ObjectToId(elId, element);
			}

			while(n)
			{
				SafeNode snode((Element*)n);
				DOMString *selector = snode.GetAttribute("selector")->Value(&empty);
				DOMString *id = snode.GetAttribute("id")->Value(&empty);
				DOMString *className = snode.GetAttribute("className")->Value(&empty);
				DOMString *pseudoClass = snode.GetAttribute("pseudoClass")->Value(&empty);

				bool apply = false;
				if (*selector == "" || *selector == elSelector)
				{
					if (*className == "" || *className == elClassName)
					{
						if (*pseudoClass == "" || *pseudoClass == elPseudoClass)
						{
							if (*id == "" || *id == elId)
							{
								apply = true;
							}
						}
					}
				}

				if (apply)
				{
					/*
					printf("%s apply %s .%s:%s #%s\n",
						f->GetName(),
						selector->c_str(),
						className->c_str(),
						pseudoClass->c_str(),
						id->c_str());
					*/
					ApplyStyle(f->frameStyle, (Element*)n);
				}

				n = n->next;
			}

			GetLayoutXml(f->frameStyle.layout, element);
		}

		n = element->FirstChild();
		while(n)
		{
			ApplyStyle((Element*)n);
			n= n->next;
		}
		return true;
	}

	bool StyleSheet::ApplyStyle(FrameStyle &fs, Element *style)
	{
		if (!style)
			return false;

		SafeNode snode(style);
		GetLayoutXml(fs.layout, snode.GetElement("layout")->Node());
		GetBorderStyleXml(fs.borderStyle, snode.GetElement("border")->Node());
		GetBordersXml(fs.border, snode.GetElement("border")->Node());
		GetBordersXml(fs.margin, snode.GetElement("margin")->Node());
		GetBordersXml(fs.padding, snode.GetElement("padding")->Node());
		GetGradientXml(fs.gradient, snode.GetElement("gradient")->Node());
		return true;
	}
}
