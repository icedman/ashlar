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

using namespace Ash;

namespace Layout
{
	class Frame;

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
		virtual const char* GetName() { return "frame"; } //!< Get frame name

		virtual bool Layout(); //!< Layout method. Layout frames override this method
		virtual bool OnEvent(int eventId, void *); //!< Event method. Event listeners override this method
		virtual Frame* Create() { return new Frame(); }

		FrameList* GetFrames() { return &frames; } //!< Get child frames list

		static void FreeFrames(Frame *frame, bool freeSelf = false);

		Frame();
		virtual ~Frame();

		void Dump();

	public:
		FrameStyle frameStyle; //!< Frame style information

	private:
		Frame *parentFrame;
		FrameList frames;
	};

}