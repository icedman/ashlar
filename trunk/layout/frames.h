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

#include <debug.h>
#include <common.h>
#include <rect.h>
#include <list.h>
#include <layout/framestyle.h>
#include <dom/domstring.h>
#include <dom/element.h>
#include <dom/uievent.h>
#include <render/render.h>

using namespace Ash;

namespace Layout
{
	class Frame;

	//! List of frames
	typedef List<Frame> FrameList;

	//! Base class for all frame objects
	class Frame : public Node<Frame>, public Dom::EventListener
	{
	public:
		Frame();
		virtual ~Frame();

		virtual Frame* Create() { return new Frame(); }
		virtual const char* GetName() { return "frame"; } //!< Get frame name
		virtual bool IsType(unsigned int frameType) { return (frameType == 0); }

		virtual bool Initialize();

		// frame visual state
		virtual void SetState(int state);
		virtual int GetState() { return frameState; }

		// frame tree
		virtual bool SetParent(Frame*);		//!< Sets the parent of a frame
		virtual Frame* GetRoot();				//!< Get root frame
		virtual Frame* GetParent();			//!< Get parent of a frame
		virtual Frame* GetParent(int frameType);	 //!< Get parent frame that is of given type
		virtual bool AddFrame(Frame*);		//!< Add child frame
		virtual bool RemoveFrame(Frame*);	//!< Remove child frame
		FrameList* GetFrames() { return &frames; } //!< Get child frames list

		// layout
		virtual bool GetRect(Rect*);			//!< Get frame rect
		virtual bool GetBorderRect(Rect*);	//!< Get frame border rect
		virtual bool GetContentRect(Rect*);	//!< Get frame's content rect
		virtual bool SetRect(const Rect*);	//!< Set frame rect
		virtual Frame* GetFrameFromPoint(Point p); //!< Get frame from point

		// layout
		virtual bool Prelayout();	//!< Prelayout method. Child frames set their preferred dimensions
		virtual bool Layout();		//!< Layout method. Layout frames override this method. Parent frames set child dimensions
		virtual void Relayout();	//!< Tells root window to recalculate layout
		virtual void Restyle();		//!< Reapplies the stylesheet of the frame

		// rendering
		virtual void Draw(Render::Rasterizer *render);
		virtual void DrawFrame(Render::Rasterizer *render, double x, double y, double x2, double y2, Dom::DOMString *text = 0);
		virtual void DrawFrameText(Render::Rasterizer *render, Dom::DOMString *text, double x, double y, double x2, double y2);
		virtual void DrawChildren(Render::Rasterizer *render);
		virtual void Redraw();

		// dom element
		virtual Dom::Element* SetElement(Dom::Element *e);
		virtual Dom::Element* GetElement() { return element; }
		virtual Dom::DOMString* GetText();

		// events
		virtual bool RegisterEventListeners();
		virtual bool PropagateUIEvent(Dom::Event *evt);
		virtual bool PropagateMouseEvent(Dom::MouseEvent *evt);
		virtual bool PropagateKeyEvent(Dom::KeyEvent *evt);
		virtual void HandleEvent(Dom::Event *evt);

		virtual void Free();

#ifdef DEBUG
		void Dump();

		inline int CountFrames(int lastCount = 0)
		{
			lastCount++;
			Frame *f = frames.GetFirst();
			while(f)
			{
				lastCount = f->CountFrames(lastCount);
				f = f->next;
			}
			return lastCount;
		}
#endif

	public:
		FrameStyle frameStyle;	//!< Frame style information

	protected:
		Frame *parentFrame;
		FrameList frames;
		unsigned short frameState;
		unsigned short mouseState;

		Dom::Element* element;
	};

}