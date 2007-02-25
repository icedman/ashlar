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
	const unsigned short STATE_NORMAL = 0;
	const unsigned short STATE_PRESSED = 1;
	const unsigned short STATE_DISABLED = 2;
	const unsigned short STATE_ACTIVE = 3;
	const unsigned short STATE_HOVER = 4;
	const unsigned short STATE_VISITED = 5;
	
	//! Alignment values
	const unsigned short ALIGN_LEFT = 0;
	const unsigned short ALIGN_RIGHT = 1;
	const unsigned short ALIGN_CENTER = 2;
	const unsigned short ALIGN_TOP = 0;
	const unsigned short ALIGN_BOTTOM = 1;
	const unsigned short ALIGN_MIDDLE = 2;

	//! Graident style
	const unsigned short FILL_SOLID = 0;
	const unsigned short FILL_GRADIENT_LINEAR = 1;
	const unsigned short FILL_GRADIENT_RADIAL = 2;

	//! Font
	const unsigned short FONT_WEIGHT_NORMAL = 0;
	const unsigned short FONT_WEIGHT_BOLD = 1;

	//! Position
	const unsigned short POSITION_FLOW = 0;
	const unsigned short POSITION_ABSOLUTE = 1;
	const unsigned short POSITION_RELATIVE = 2;

	//! Image extend
	const unsigned short EXTEND_NONE = 0;
	const unsigned short EXTEND_REPEAT = 1;
	const unsigned short EXTEND_STRETCH = 2;
	const unsigned short EXTEND_HIDDEN = 3;

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
		long color;
		long fontId;
		short size;
		short weight;
		short italized;
		short outlined;
		long outlineColor;
		short outlineWidth;
		Shadow shadow;
	} Font;

	typedef struct Background
	{
		long imageId;
		short extend;
		Borders slice;
		Borders sliceExtend;
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
		short position;
		bool display;
		bool visible;
		// calculated values
		short presetX;
		short presetY;
		short presetWidth;
		short presetHeight;
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