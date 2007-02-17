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

#pragma once

#include "document.h"
#include "framestyle.h"
#include "list.h"

using namespace Ash;
using namespace Layout;

namespace Dom
{
	inline int StringToAlign(DOMString *str, int defaultValue);
	inline int StringToStyle(DOMString *str, int defaultValue);
	inline long StringToColor(DOMString *str, long defaultValue);
	void GetBorderStyleXml(BorderStyle &bs, DOMNode *el);
	void GetLayoutXml(LayoutInfo &li, DOMNode *el);
	void GetBordersXml(Borders &br, DOMNode *el);
	void GetGradientXml(Gradient &gr, DOMNode *el);
	void GetFontXml(Font &fn, DOMNode *el);

	class StyleSheet : public DOMDocument
	{
	public:
	
		bool LoadFile(const char* filename);
		bool Load(Element *element);
		bool AddStyle(Element *style);

		bool ApplyStyle(Element *element);
		bool ApplyStyle(FrameStyle &fs, Element *style);
		bool GetStyleName(Element *element, DOMString &selector, DOMString &className, DOMString &id, DOMString &pseudoClass);
		bool GetSelector(Element *element, DOMString &selector, DOMString &className, DOMString &id, DOMString &pseudoClass);

	};
}