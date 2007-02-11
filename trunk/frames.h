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
#include "framestyle.h"
#include "domnode.h"
#include "events.h"

using namespace Ash;
using namespace Dom;

namespace Layout
{
	class Frame;

	//! List of frames
	typedef List<Frame> FrameList;

	//! Base class for all frame objects
	class Frame : public Events::EventListener, public Node<Frame>
	{
	public:
		virtual bool SetParent(Frame*);		//!< Sets the parent of a frame
		virtual Frame* GetRoot();				//!< Get root frame
		virtual Frame* GetParent();			//!< Get parent of a frame
		virtual Frame* GetParent(int frameType);	 //!< Get parent frame that is of given type
		virtual bool AddFrame(Frame*);		//!< Add child frame
		virtual bool RemoveFrame(Frame*);	//!< Remove child frame
		virtual bool GetRect(Rect*);			//!< Get frame rect
		virtual bool GetBorderRect(Rect*);	//!< Get frame border rect
		virtual bool GetContentRect(Rect*);	//!< Get frame's content rect
		virtual bool SetRect(const Rect*);	//!< Set frame rect
		virtual const char* GetName() { return "frame"; } //!< Get frame name

		virtual bool Layout(); //!< Layout method. Layout frames override this method
		virtual bool OnEvent(int eventId, void *);
		virtual Frame* Create() { return new Frame(); }
		virtual void SetState(int state);
		virtual int GetState() { return frameState; }
		virtual void Redraw();
		// frameType 0 is base FRAME
		virtual bool IsType(int frameType) { return (frameType == 0); }

		FrameList* GetFrames() { return &frames; } //!< Get child frames list

		virtual void Free();

		Frame();
		virtual ~Frame();

		void Dump();

	public:
		FrameStyle frameStyle;	//!< Frame style information

	private:
		Frame *parentFrame;
		FrameList frames;
		int frameState;

		DOMNode* element;
	};

}