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

#include "framestyle.h"
#include "list.h"
#include "element.h"

using namespace Dom;

namespace Layout
{

	//! Style class
	class Style : public Ash::Node<Style>
	{
	public:
		bool Build(Element *e);
		bool Apply(FrameStyle &fs);
		bool GetStyleName(Element *e);
		bool IsEqual(Style *s);

		static Style* Create(Element *e);

		DOMString id;				//!< id
		DOMString selector;		//!< selector, the element's tag name
		DOMString className;		//!< class name
		DOMString pseudoClass;	//!< pseudo class

		FrameStyle style;

	public:

		//! Get layout from xml tree
		static void GetLayoutXml(LayoutInfo &li, DOMNode *el);
		//! Get gradient from xml tree
		static void GetGradientXml(Gradient &gr, DOMNode *el);
		//! Get border from xml tree
		static void GetBordersXml(Borders &br, DOMNode *el);
		//! Get border style from xml tree
		static void GetBorderStyleXml(BorderStyle &bs, DOMNode *el);

		//! Convert DOMString to alignment value
		static inline int StringToAlign(DOMString *str, int defaultValue = 0);
		//! Convert DOMStrng to color value
		static inline long StringToColor(DOMString *str, long defaultValue);
		//! Convert DOMString to style
		static inline int StringToStyle(DOMString *str, int defaultValue = 0);

	};

	//! Style manager
	class StyleManager : public List<Style>
	{
	public:
		StyleManager();
		~StyleManager();

		Style* AddStyle(Element *element);
		Style* GetStyle(Element *element);
		bool LoadStyle(Element *element);
		void Free();

		void Dump();
	};

}