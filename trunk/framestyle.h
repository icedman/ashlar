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
#include "domnode.h"

using namespace Ash;
using namespace Dom;

namespace Layout
{

#define UNASSIGNED (-0xEE)
#define ISASSIGNED(x) (x!=UNASSIGNED)

	//! Alignment values
	const unsigned short LEFT = 0;
	const unsigned short RIGHT = 1;
	const unsigned short CENTER = 2;
	const unsigned short TOP = 0;
	const unsigned short BOTTOM = 1;
	const unsigned short MIDDLE = 2;

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
		long colors[8];
		int colorCount;
	} Gradient;

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

	typedef struct SVG
	{
		long svgId;
		int x;
		int y;
		int scaleX;
		int scaleY;
	} SVG;

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
		Background background;
		Gradient gradient;
		SVG svg;
	} FrameStyle;

	//! Frame calculation class
	/*!
	Simplifies layout information calculations
	Unassigned values such as margins are evaluated
	*/
	class FrameTool
	{
	public:
		static void SetDefaults(FrameStyle &fs);
		static void SetStyleFromXml(FrameStyle &fs, DOMNode *el);
		static void SetLayoutFromXml(LayoutInfo &li, DOMNode *el);
		
		static void GetMetrics(LayoutInfo &li, int &x, int &y, int &w, int &h);
		static void GetBorders(Borders &borders, int &l, int &t, int &r, int &b);
		static void GetContentOffsets(FrameStyle &frameStyle, int &l, int &t, int &r, int &b);
		static inline int StringToAlign(DOMString *str, int defaultValue = 0);
		static inline long StringToColor(DOMString *str, int *r, int *g, int *b, int *a);
	};
}