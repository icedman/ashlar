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

#include "stylemanager.h"
#include "safenode.h"
#include "frames.h"

namespace Layout
{
	// Style
	bool Style::IsEqual(Style *s)
	{
		if (id.size() && id != s->id)
			return false;
		if (selector.size() && selector != s->selector)
			return false;
		if (className.size() && className != s->className)
			return false;
		if (pseudoClass.size() && pseudoClass != s->pseudoClass)
			return false;
		return true;
	}

	bool Style::GetStyleName(Element *e)
	{
		// stylesheet selector.class:pseudoClass#id

		selector = "";
		id = "";
		className = "";
		pseudoClass = "";

		SafeNode snode(e);

		DOMString *name = snode.GetValue(&DOMString("name"))->Value();
		DOMString tmpName("#");

		if (!name)
		{
			DOMNode *p = e->ParentNode();
			if (p)
			{
				if (p->nodeName != "stylesheet")
				{
					char address[32] = "";
					sprintf(address, "&%d", p);
					tmpName.append(address);
					name = &tmpName;
				}
			}
		}

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
		if (psIdx!=-1)
		{
			DOMString tmp(*name, psIdx + 1);
			any = tmp.find_first_of(":.#");
			any = (any!=-1) ? any : tmp.size();
			pseudoClass = DOMString(tmp, 0, any);
		}
		if (idIdx!=-1)
		{
			DOMString tmp(*name, idIdx + 1);
			any = tmp.find_first_of(":.#");
			any = (any!=-1) ? any : tmp.size();
			id = DOMString(tmp, 0, any);
		}

		return true;
	}

	bool Style::GetSelector(Element *e)
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
		} else {
			char address[32] = "";
			sprintf(address, "&%d", e);
			id = address;
		}

		// class
		tmp = snode.GetAttribute(&DOMString("class"))->Value();
		if (tmp)
		{
			className = *tmp;
		}

		return true;
	}

	bool Style::Build(Element *e)
	{
		SetStyleDefaults(style);
		if (!GetStyleName(e))
			return false;

		SafeNode snode(e);
		GetLayoutXml(style.layout, snode.GetElement(&DOMString("layout"))->Node());
		GetGradientXml(style.gradient, snode.GetElement(&DOMString("gradient"))->Node());
		GetBordersXml(style.margin, snode.GetElement(&DOMString("margin"))->Node());
		GetBordersXml(style.border, snode.GetElement(&DOMString("border"))->Node());
		GetBordersXml(style.padding, snode.GetElement(&DOMString("padding"))->Node());
		GetBorderStyleXml(style.borderStyle,snode.GetElement(&DOMString("border"))->Node());

		return true;
	}

	bool Style::Apply(FrameStyle &fs)
	{
		LayoutInfo t = fs.layout;
		fs = style;
		fs.layout = t;
		printf("apply %d\n", style.gradient.colorCount);
		return true;
	}

	Style* Style::Create(Element *e)
	{
		Style *s = new Style();
		if (!s->Build(e))
		{
			delete s;
			return 0;
		}
		return s;
	}

	int Style::StringToAlign(DOMString *str, int defaultValue)
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

	int Style::StringToStyle(DOMString *str, int defaultValue)
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

	long Style::StringToColor(DOMString *str, long defaultValue)
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

	void Style::GetBorderStyleXml(BorderStyle &bs, DOMNode *el)
	{
		if (!el)
			return;

		SafeNode snode((Element*)el);
		//bs.style = snode.GetValue(&DOMString("style"))->ValueInt(bs.style);
		bs.color = StringToColor(snode.GetValue(&DOMString("color"))->Value(), 0);
		//bs.imageId = snode.GetValue(&DOMString("imageId"))->ValueInt(bs.imageId);
		GetBordersXml(bs.radius, snode.GetElement(&DOMString("radius"))->Node());
		//bs.bevelStyle = snode.GetValue(&DOMString("bevelStyle"))->ValueInt(bs.bevelStyle);
		//bs.bevelColor = StringToColor(snode.GetValue(&DOMString("bevelColor"))->Value(), 0);
		//bs.bevelWidth = snode.GetValue(&DOMString("bevelWidth"))->ValueInt(bs.bevelWidth);
	}

	void Style::GetLayoutXml(LayoutInfo &li, DOMNode *el)
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

	void Style::GetBordersXml(Borders &br, DOMNode *el)
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

	void Style::GetGradientXml(Gradient &gr, DOMNode *el)
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

	void Style::Dump()
	{
		printf("selector:%s\nclass:%s\npseudo:%s\nid:%s\n",
			selector.c_str(),
			className.c_str(),
			pseudoClass.c_str(),
			id.c_str());
	}

	void Style::DumpStyle()
	{
		printf("border: %d\ngradient: %d\n",
			style.border.width,
			style.gradient.colorCount);
	}

	// Style manager
	StyleManager::StyleManager()
	{
	}

	StyleManager::~StyleManager()
	{
		Free();
	}

	Style* StyleManager::AddStyle(Element *element)
	{
		if (element->nodeName != DOMString("style"))
		{
			return 0;
		}

		Style *s = GetStyle(element);
		if (s)
		{
			// update style
			s->Build(element);
			printf("update style\n");
		} else {
			s = Style::Create(element);
			if (s)
			{
				Push(s);
			}
		}
		return s;
	}

	Style* StyleManager::GetStyle(Style *style)
	{
		Style *s = GetFirst();
		while(s)
		{
			if (s->IsEqual(style))
				return s;
			s = s->next;
		}
		return 0;
	}

	Style* StyleManager::GetStyle(Element *element)
	{
		Style sname;
		sname.GetStyleName(element);
		return GetStyle(&sname);
	}

	bool StyleManager::LoadStyle(Element *element)
	{
		AddStyle(element);
		if (element->HasChildNodes())
		{
			DOMNode *n = element->FirstChild();
			while(n)
			{
				LoadStyle((Element*)n);
				n = n->NextSibling();
			}
		}
		return true;
	}

	void StyleManager::Free()
	{
		Style *s = 0;
		while(s = GetFirst())
		{
			Remove(s);
			delete s;
		}
	}

	bool StyleManager::ApplyStyle(Element *element)
	{
		Frame *f = (Frame*)element->GetData();
		if (f)
		{
			Style sname;
			sname.GetSelector(element);
			sname.Dump();
			Style *s = GetStyle(&sname);
			if (s)
			{
				s->DumpStyle();
				s->Apply(f->frameStyle);
			}
			if (sname.id.size())
			{
				sname.id = "";
				Style *s = GetStyle(&sname);
				if (s)
				{
					s->DumpStyle();
					s->Apply(f->frameStyle);
				}
			}
		}

		DOMNode *n = element->FirstChild();
		while(n)
		{
			ApplyStyle((Element*)n);
			n = n->NextSibling();
		}

		return true;
	}

	void StyleManager::Dump()
	{
		Style *s = GetFirst();
		while(s)
		{
			s->Dump();
			s->DumpStyle();
			s = s->next;
		}
	}
}
