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

#include "common.h"
#include "rect.h"
#include "list.h"

using namespace Ash;

namespace Layout
{

#define UNASSIGNED (-0xEE)
#define ISASSIGNED(x) (x!=UNASSIGNED)

	class Frame;

	//! Alignment values
	const unsigned short LEFT = 0;
	const unsigned short RIGHT = 1;
	const unsigned short CENTER = 2;
	const unsigned short TOP = 0;
	const unsigned short BOTTOM = 1;
	const unsigned short MIDDLE = 2;

	//! Layout information data structure
	typedef struct FrameInfo
	{
		int x;
		int y;
		int width;
		int height;
		int flex;
		int align;
		int verticalAlign;
		int margin;
		int marginLeft;
		int marginTop;
		int marginRight;
		int marginBottom;
		int border;
		int borderLeft;
		int borderTop;
		int borderRight;
		int borderBottom;
		int padding;
		int paddingLeft;
		int paddingTop;
		int paddingRight;
		int paddingBottom;
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

	typedef struct FrameStyle
	{
		// foreground
		long color;
		long fontId;
		int fontStyle;
		int fontSize;
		// background
		long backgroundColor;
		long backgroundImageId;
		int backgroundStyle;
		int backgroundImageX;
		int backgroundImageY;
		// border
		long borderColor;
		long borderImageId;
		int borderStyle;
		int borderRadius;
		int borderRadiusLeftTop;
		int borderRadiusLeftBottom;
		int borderRadiusRightTop;
		int borderRadiusRightBottom;
		// bevel
		long bevelColor;
		int bevelWidth;
		int bevelStyle;
		// shadow
		long shadowColor;
		int shadowStyle;
		int shadowX;
		int shadowY;
		long fontShadowColor;
		int fontShadowStyle;
		int fontShadowX;
		int fontShadowY;
	} FrameStyle;

	//! Frame calculation class
	/*!
	Simplifies layout information calculations
	Unassigned values such as margins are evaluated
	*/
	class FrameTool
	{
	public:
		static void SetDefaults(LayoutInfo &li);
		static void SetDefaults(FrameStyle &fs);
		static void GetMetrics(LayoutInfo &li, int &x, int &y, int &w, int &h);
		static void GetMargins(LayoutInfo &li, int &l, int &t, int &r, int &b);
		static void GetBorders(LayoutInfo &li, int &l, int &t, int &r, int &b);
		static void GetContentOffsets(LayoutInfo &li, int &l, int &t, int &r, int &b);
	};

	//! List of frames
	typedef List<Frame> FrameList;

	//! Base class for all frame objects
	class Frame : public Node<Frame>
	{
	public:
		virtual bool SetParent(Frame*);		//!< Sets the parent of a frame
		virtual Frame* GetParent();			//!< Get parent of a frame
		virtual Frame* GetRoot();				//!< Get root frame
		virtual bool AddFrame(Frame*);		//!< Add child frame
		virtual bool RemoveFrame(Frame*);	//!< Remove child frame
		virtual bool GetRect(Rect*);			//!< Get frame rect
		virtual bool GetBorderRect(Rect*);	//!< Get frame border rect
		virtual bool GetContentRect(Rect*);	//!< Get frame's content rect
		virtual bool SetRect(const Rect*);	//!< Set frame rect
		virtual const char* GetName() { return "Frame"; } //!< Get frame name

		virtual bool Layout(); //!< Layout method. Layout frames override this method
		virtual bool OnEvent(int eventId, void *); //!< Event method. Event listeners override this method

		FrameList* GetFrames() { return &frames; } //!< Get child frames list

		static void FreeFrames(Frame *frame, bool freeSelf = false);

		Frame();
		virtual ~Frame();

	public:
		LayoutInfo layoutInfo; //!< Frame layout information
		FrameStyle frameStyle; //!< Frame style information

	private:
		Frame *parentFrame;
		FrameList frames;
	};

}