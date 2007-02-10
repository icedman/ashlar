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

#include "rect.h"

using namespace Ash;

namespace Layout
{

#define UNASSIGNED (-0xEE)
#define ISASSIGNED(x) (x!=UNASSIGNED)
#define GRADIENT_COLOR_LIMIT 4

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
		int width;
		int left;
		int top;
		int right;
		int bottom;
	} Borders;

	typedef struct Shadow
	{
		int style;
		long color;
		int x;
		int y;
	} Shadow;

	typedef struct Font
	{
		// todo: more detail
		int style;
		long color;
		long fontId;
		int size;
		long outlineColor;
		int outlineWidth;
		Shadow shadow;
	} Font;

	typedef struct Background
	{
		int style;
		long color;
		long imageId;
		int imageX;
		int imageY;
	} Background;

	typedef struct Gradient
	{
		int style;
		int x;
		int y;
		int radius;
		int x2;
		int y2;
		int radius2;
		long colors[GRADIENT_COLOR_LIMIT];
		int offsets[GRADIENT_COLOR_LIMIT];
		int colorCount;
	} Gradient;

	typedef struct SVG
	{
		long svgId;
		int x;
		int y;
		int scaleX;
		int scaleY;
	} SVG;

	typedef struct BorderStyle
	{
		int style;
		int borderWidth;
		long color;
		long imageId;
		int radius;
		int radiusLeftTop;
		int radiusLeftBottom;
		int radiusRightTop;
		int radiusRightBottom;
		int bevelStyle;
		long bevelColor;
		int bevelWidth;
	} BorderStyle;

	//! Layout information
	typedef struct LayoutInfo
	{
		int x;
		int y;
		int width;
		int height;
		int flex;
		int align;
		int verticalAlign;
		bool floating;
		bool display;
		bool visible;
		// calculated values
		int totalFlex;
		int totalChildWidths;
		int totalChildHeights;
		int maxChildWidth;
		int maxChildHeight;
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
		SVG svg;
	} FrameStyle;

	//! Set frame style defaults
	void SetStyleDefaults(FrameStyle &fs);
	//! Get layout metrics considering unassigned values
	void GetMetrics(LayoutInfo &li, int &x, int &y, int &w, int &h);
	//! Get style borders considering unassigned values
	void GetBorders(Borders &borders, int &l, int &t, int &r, int &b);
	//! Get frame content offsets (margin + border + padding)
	void GetContentOffsets(FrameStyle &frameStyle, int &l, int &t, int &r, int &b);
}