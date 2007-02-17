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

#include <events/events.h>
#include <rect.h>

using namespace Ash;

namespace Events
{
	// Event
	Event::Event(int eid, EventListener *listener)
	{
		Attach(eid, listener);
	}

	bool Event::Attach(int eid, EventListener *plistener)
	{
		eventId = eid;
		listener = plistener;
		return true;
	}

	// Event Manager
	EventManager::EventManager()
	{
	}

	EventManager::~EventManager()
	{
		ClearListeners();
	}

	void EventManager::ClearListeners()
	{
		Event *e = GetFirst();
		while(e = GetFirst())
		{
			Remove(e);
			delete e;
		}
	}

	bool EventManager::AddListener(int eventId, EventListener *pListener)
	{
		if (HasEvent(&Event(eventId, pListener)))
			return true;

		return AddListener(new Event(eventId, pListener));
	}

	bool EventManager::AddListener(Event* pEvent)
	{
		return Push(pEvent);
	}

	bool EventManager::RemoveListener(Event* pEvent)
	{
		return Remove(pEvent);
	}

	bool EventManager::HasEvent(Event *pEvent)
	{
		Event *e = GetFirst();
		while(e)
		{
			if (pEvent->eventId == e->eventId && pEvent->listener == e->listener)
				return true;
			e = e->next;
		}
		return false;
	}

	// MouseEvents
	bool MouseEvents::AddListener(Event *e)
	{
		// filter
		if (e->eventId == ONMOUSEDOWN)
			return EventManager::AddListener(e);
		if (e->eventId == ONMOUSEUP)
			return EventManager::AddListener(e);
		if (e->eventId == ONMOUSEMOVE)
			return EventManager::AddListener(e);
		if (e->eventId == ONMOUSEENTER)
			return EventManager::AddListener(e);
		if (e->eventId == ONMOUSEOUT)
			return EventManager::AddListener(e);
		return false;
	}

	bool MouseEvents::OnMouseEvent(int eventId, int button, int x, int y)
	{
		Event *e = GetFirst();
		while(e)
		{
			EventListener *listener = e->listener;
			if (e->eventId == eventId)
			{
				MouseInfo mInfo;
				mInfo.point = Point(x,y);
				mInfo.button = button;
				listener->OnEvent(eventId, (void*)&mInfo);
			}
			e = e->next;
		}
		return true;
	}
}
