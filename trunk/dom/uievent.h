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

#include <dom/event.h>
#include <dom/eventtypes.h>
#include <iwindow.h>

namespace Dom
{

	const unsigned int FOCUS_IN  = 0;
	const unsigned int FOCUS_OUT = 1;
	const unsigned int FOCUS_ACTIVATE = 2;

	/*
	// IWindow mouse events
	const unsigned int MOUSE_CLICK	= 0;
	const unsigned int MOUSE_DOWN		= 1;
	const unsigned int MOUSE_UP		= 2;
	const unsigned int MOUSE_OVER		= 3;
	const unsigned int MOUSE_MOVE		= 4;
	const unsigned int MOUSE_OUT		= 5;
	*/

	class UIEvent : public Event
	{
	public:
		EVENTGROUP(UI_EVENTS)
	};

	class MouseEvent : public UIEvent
	{
	public:
		EVENTGROUP(MOUSE_EVENTS)

			void InitEvent(unsigned int eventTypeArg, bool canBubbleArg, bool cancellableArg,
			long screenX,
			long screenY,
			long clientX,
			long clientY,
			bool ctrlKey,
			bool shiftKey,
			bool altKey,
			bool metaKey,
			unsigned short button
			);

		long screenX;
		long screenY;
		long clientX;
		long clientY;
		bool ctrlKey;
		bool shiftKey;
		bool altKey;
		bool metaKey;
		unsigned short button;
	};

	class KeyEvent : public UIEvent
	{
	public:
		EVENTGROUP(KEY_EVENTS)
	};
}