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
#include "safenode.h"

using namespace Dom;

namespace Layout
{
	//! Get style from xml tree
	void GetStyleXml(FrameStyle &fs, DOMNode *el);
	//! Get layout from xml tree
	void GetLayoutXml(LayoutInfo &li, DOMNode *el);
	//! Get gradient from xml tree
	void GetGradientXml(Gradient &gr, DOMNode *el);
	//! Get border from xml tree
	void GetBordersXml(Borders &br, DOMNode *el);
	//! Get border style from xml tree
	void GetBorderStyleXml(BorderStyle &bs, DOMNode *el);

	//! Convert DOMString to alignment value
	inline int StringToAlign(DOMString *str, int defaultValue = 0);
	//! Convert DOMStrng to color value
	inline long StringToColor(DOMString *str, long defaultValue);
	//! Convert DOMString to style
	inline int StringToStyle(DOMString *str, int defaultValue = 0);
}