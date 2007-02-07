#pragma once

#include "common.h"
#include "rect.h"

#include <vector>

namespace Ash
{

#define UNASSIGNED (-0xEE)
#define ISASSIGNED(x) (x!=UNASSIGNED)

	typedef enum
	{
		LEFT,
		RIGHT,
		CENTER,
		TOP,
		BOTTOM,
		MIDDLE
	} Align;

	typedef struct LayoutInfo
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
		// calculations
		int totalFlex;
		int totalChildWidths;
		int totalChildHeights;
		int maxChildWidth;
		int maxChildHeight;
		Rect rect;
	} LayoutInfo;

	class Frame;
	typedef std::vector<Frame*> FrameList;

	class Frame
	{
	public:
		// Frame
		virtual bool SetParent(Frame*);
		virtual Frame* GetParent();
		virtual Frame* GetRoot();
		virtual bool AddFrame(Frame*);
		virtual bool RemoveFrame(Frame*);
		virtual bool GetRect(Rect*);
		virtual bool GetBorderRect(Rect*);
		virtual bool GetClientRect(Rect*);
		virtual bool SetRect(const Rect*);
		virtual const char* GetName() { return "Frame"; }
		// Layout
		virtual bool Layout();

		Frame();
		virtual ~Frame();
		FrameList* GetFrames() { return &frames; }

	public:
		// Layout
		LayoutInfo layoutInfo;

	private:
		Frame *parentFrame;
		FrameList frames;
	};

}