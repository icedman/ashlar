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

#include "frames.h"
#include "list.h"

using namespace Ash;
using namespace Layout;

namespace Events
{
	typedef enum
	{
		ONMOUSEDOWN,
		ONMOUSEUP,
		ONMOUSEMOVE,
		ONMOUSEENTER,
		ONMOUSEOUT
	} EventIds;

	//! Event class
	class Event : public Node<Event>
	{
	public:

		bool Attach(int eventId, Frame *); //!< Attach frame to an event

		Event(int eventId = 0, Frame * = 0);
		virtual ~Event() {};

	public:
		int eventId;
		Frame  *frame;
	};

	//! Events manager
	class EventManager : public List<Event>
	{
	public:
		bool AddListener(Event *);
		bool RemoveListener(Event *);
	};

	//! Mouse events manager
	class MouseEvents : public EventManager
	{
	public:
		bool OnMouseEvent(int eventId, int button, int x, int y);
	};

}
