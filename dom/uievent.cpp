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

#include <dom/uievent.h>
#include <dom/eventtypes.h>

namespace Dom
{
	void MouseEvent::InitEvent(unsigned int eventTypeArg, bool canBubbleArg, bool cancellableArg,
		long screenXArg,
		long screenYArg,
		long clientXArg,
		long clientYArg,
		bool ctrlKeyArg,
		bool shiftKeyArg,
		bool altKeyArg,
		bool metaKeyArg,
		unsigned short buttonArg
		)
	{
		type = eventTypeArg;
		bubbles = canBubbleArg;
		cancellable = cancellableArg;
		screenX = screenXArg;
		screenY = screenYArg;
		clientX = clientXArg;
		clientY = clientYArg;
		ctrlKey = ctrlKeyArg;
		shiftKey = shiftKeyArg;
		altKey = altKeyArg;
		metaKey = metaKeyArg;
		button = buttonArg;
	}
}