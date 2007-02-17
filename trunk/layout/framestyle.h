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

#include <rect.h>
#include <list.h>

using namespace Ash;

namespace Layout
{

#define UNASSIGNED (-0xEE)
#define ISASSIGNED(x) (x!=UNASSIGNED)
#define GRADIENT_COLOR_LIMIT 4

	//! Frame pseudo classes (button:pressed)
	const unsigned short NORMAL = 0;
	const unsigned short PRESSED = 1;
	const unsigned short DISABLED = 2;
	const unsigned short ACTIVE = 3;
	const unsigned short HOVER = 4;
	const unsigned short VISITED = 5;
	
	//! Alignment values
	const unsigned short LEFT = 0;
	const unsigned short RIGHT = 1;
	const unsigned short CENTER = 2;
	const unsigned short TOP = 0;
	const unsigned short BOTTOM = 1;
	const unsigned short MIDDLE = 2;

	//! Graident style
	const unsigned short LINEAR = 1;
	const unsigned short RADIAL = 2;

	//! Background
	const unsigned short FILL  = 1;
	const unsigned short IMAGE = 2;

	//! Layout information data structure
	typedef struct Borders
	{
		short width;
		short left;
		short top;
		short right;
		short bottom;
	} Borders;

	typedef struct Shadow
	{
		short style;
		long color;
		short x;
		short y;
	} Shadow;

	typedef struct Font
	{
		// todo: more detail
		short style;
		long color;
		long fontId;
		short size;
		long outlineColor;
		short outlineWidth;
		Shadow shadow;
	} Font;

	typedef struct Background
	{
		short style;
		long color;
		long imageId;
		short imageX;
		short imageY;
	} Background;

	typedef struct Gradient
	{
		short style;
		short x;
		short y;
		short radius;
		short x2;
		short y2;
		short radius2;
		long colors[GRADIENT_COLOR_LIMIT];
		short offsets[GRADIENT_COLOR_LIMIT];
		short colorCount;
	} Gradient;

	typedef struct SVG
	{
		long svgId;
		short x;
		short y;
		short scaleX;
		short scaleY;
	} SVG;

	typedef struct BorderStyle
	{
		short style;
		long color;
		long imageId;
		Borders radius;
		short bevelStyle;
		long bevelColor;
		short bevelWidth;
	} BorderStyle;

	//! Layout information
	typedef struct LayoutInfo
	{
		short x;
		short y;
		short width;
		short height;
		short flex;
		short align;
		short verticalAlign;
		bool floating;
		bool display;
		bool visible;
		// calculated values
		short totalFlex;
		short totalChildWidths;
		short totalChildHeights;
		short maxChildWidth;
		short maxChildHeight;
		Rect rect;
	} LayoutInfo;

	//! Frame style
	typedef struct FrameStyle
	{
		LayoutInfo layout;
		Borders margin;
		Borders border;
		Borders padding;
		Font font;
		BorderStyle borderStyle;
		Background bgImage;
		Gradient gradient;
	} FrameStyle;

	//! Set frame style defaults
	void SetStyleDefaults(FrameStyle &fs);
	//! Get layout metrics considering unassigned values
	void GetMetrics(LayoutInfo &li, int &x, int &y, int &w, int &h);
	//! Get style borders considering unassigned values
	void GetBorders(Borders &borders, int &l, int &t, int &r, int &b);
	//! Get frame content offsets (margin + border + padding)
	void GetContentOffsets(FrameStyle &frameStyle, int &l, int &t, int &r, int &b);
	//! Get maximum border;
	int GetMaxBorder(Borders &borders);

}