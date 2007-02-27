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

#include <iwindow.h>
#include <dom/uievent.h>
#include <dom/mutationevent.h>

namespace Dom
{
	typedef struct EventNames
	{
		unsigned int id;
		char name[32];
		char method[32];
	} EventNames;

	// event names
	const EventNames nullEventNames[] = {
		{ 0, "", "" }
	};

	// ui event names
	const EventNames uiEventNames[] = {
		{ FOCUS_IN,			"focusin",	 "onFocus" },
		{ FOCUS_OUT,		"focusout",  "onFocusOut" },
		{ FOCUS_ACTIVATE,	"activate",	 "onActivate" },
	};

	// mouse event names
	const EventNames mouseEventNames[] = {
		{ MOUSE_CLICK,	"click",		 "onClick" },
		{ MOUSE_DOWN,	"mousedown", "onMouseDown" },
		{ MOUSE_UP,		"mouseup",	 "onMouseUp" },
		{ MOUSE_OVER,	"mouseover", "onMouseOver" },
		{ MOUSE_MOVE,	"mousemove", "onMouseMove" },
		{ MOUSE_OUT,	"mouseout",  "onMouseOut" },
	};

	// key event names
	const EventNames keyEventNames[] = {
		{ 0, "", "" }
	};

	// mutation event names
	const EventNames mutationEventNames[] = {
		{ SUBTREE_MODIFIED,			"subtreeModified",	"onSubtreeModified" },
		{ NODE_INSERTED,				"nodeInserted",		"onNodeInserted" },
		{ NODE_REMOVED,				"nodeRemoved",			"onNodeRemoved" },
		{ NODE_INSERTED_TO_DOCUMENT,	"nodeRnsertedToDocument",	"onNodeRnsertedToDocument" },
		{ NODE_REMOVED_FROM_DOCUMENT,	"nodeRemovedFromDocument",	"onNodeRemovedFromDocument" },
		{ ATTRIBUTE_MODIFIED,			"attributeModified",			"onAttributeModified" },
		{ CHARACTER_DATA_MODIFIED,		"characterDataModified",	"onCharacterDataModified" },
	};

	// should be in the correct index
	const EventNames *eventNames[] = {
		nullEventNames,
		uiEventNames,
		mouseEventNames,
		keyEventNames,
		mutationEventNames
	};

	inline const EventNames* GetEventNames(unsigned int group, unsigned int type)
	{
		return &eventNames[group][type];
	}

}
