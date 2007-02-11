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

#include "list.h"
#include "rect.h"

using namespace Ash;

namespace Events
{
	class EventManager;

	//! Event Ids
	const unsigned short ONMOUSEDOWN = 1;
	const unsigned short ONMOUSEUP = 2;
	const unsigned short ONMOUSEMOVE = 3;
	const unsigned short ONMOUSEENTER = 4;
	const unsigned short ONMOUSEOUT = 5;

	//! Mouse
	const unsigned short LBUTTON = 1;
	const unsigned short RBUTTON = 2;
	const unsigned short MBUTTON = 3;

	typedef struct MouseInfo
	{
		Point point;
		unsigned short button;
	} MouseInfo;

	//! Event listener
	class EventListener
	{
	public:
		virtual bool OnEvent(int eventId, void *) { return true; }
		virtual bool RegisterEvents(EventManager *manager) { return true; }
	};

	//! Event class
	class Event : public Node<Event>
	{
	public:

		bool Attach(int eventId, EventListener *); //!< Attach frame to an event

		Event(int eventId = 0, EventListener * = 0);
		virtual ~Event() {};

	public:
		int eventId;
		EventListener *listener;
	};

	//! Events manager
	class EventManager : public List<Event>
	{
	public:
		EventManager();
		~EventManager();

		virtual bool AddListener(Event *);
		bool RemoveListener(Event *);
		void ClearListeners();
	};

	//! Mouse events manager
	class MouseEvents : public EventManager
	{
	public:
		bool AddListener(Event *);
		bool OnMouseEvent(int eventId, int button, int x, int y);
	};

}
