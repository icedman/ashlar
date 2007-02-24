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

#include <dom/document.h>
#include <layout/framestyle.h>
#include <list.h>

using namespace Ash;

namespace Layout
{
	inline int StringToAlign(Dom::DOMString *str, int defaultValue);
	inline int StringToStyle(Dom::DOMString *str, int defaultValue);
	inline long StringToColor(Dom::DOMString *str, long defaultValue);

	void GetBorderStyleXml(Layout::BorderStyle &bs, Dom::DOMNode *el);
	void GetLayoutXml(Layout::LayoutInfo &li, Dom::DOMNode *el);
	void GetBordersXml(Layout::Borders &br, Dom::DOMNode *el);
	void GetGradientXml(Layout::Gradient &gr, Dom::DOMNode *el);
	void GetFontXml(Layout::Font &fn, Dom::DOMNode *el);
	void GetImageXml(Layout::Background &img, Dom::DOMNode *el);
	void GetSliceExtendXml(Layout::Borders &ex, Dom::DOMNode *el);

	class StyleSheet : public Dom::Document
	{
	public:
	
		bool LoadFile(const char* filename);
		bool Load(Dom::Element *element);
		bool AddStyle(Dom::Element *style);

		bool ApplyStyle(Dom::Element *element);
		bool ApplyStyle(Layout::FrameStyle &fs, Dom::Element *style);
		bool GetStyleName(Dom::Element *element, Dom::DOMString &selector, Dom::DOMString &className, Dom::DOMString &id, Dom::DOMString &pseudoClass);
		bool GetSelector(Dom::Element *element, Dom::DOMString &selector, Dom::DOMString &className, Dom::DOMString &id, Dom::DOMString &pseudoClass);

	};
}