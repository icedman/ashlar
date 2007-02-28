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

#include <dom/event.h>

namespace Dom
{
		// EventTarget
		EventTarget::~EventTarget()
		{
			FreeEventTarget();
		}

		void EventTarget::AddEventListener(unsigned int groupArg, unsigned int eventTypeArg, EventListener *listener, bool capture)
		{
			EventListenerNode *n = listeners.Find(groupArg, eventTypeArg, listener, capture);
			if (n)
				return;
			listeners.Push(new EventListenerNode(groupArg, eventTypeArg, listener, capture));
		}

		void EventTarget::RemoveEventListener(unsigned int groupArg, unsigned int eventTypeArg, EventListener *listener, bool capture)
		{
			EventListenerNode *n = listeners.Find(groupArg, eventTypeArg, listener, capture);
			if (!n)
				return;
			listeners.Remove(n);
			delete n;
		}

		bool EventTarget::DispatchEvent(Event *evt)
		{
			EventListenerNode *l = listeners.GetFirst();
			while(l)
			{	
				if (l->type == evt->type &&
					 l->group == evt->GetEventGroup())
				{
					l->listener->HandleEvent(evt);
				}
				l = l->next;
			}
			return true;
		}

		void EventTarget::FreeEventTarget()
		{
			EventListenerNode *l = 0;
			while(l = listeners.GetFirst())
			{	
				listeners.Remove(l);
				delete l;
			}
		}

		// EventListenerNode
		EventListenerNode::EventListenerNode(unsigned int groupArg, unsigned int eventTypeArg, EventListener *listenerArg, bool capture)
		{
			group = groupArg;
			type = eventTypeArg;
			listener = listenerArg;
			useCapture = capture;
		}

		// EventListenerNodeList
		EventListenerNode* EventListenerNodeList::Find(unsigned int groupArg, unsigned int eventTypeArg, EventListener *listener, bool capture)
		{
			EventListenerNode *ln = GetFirst();
			while(ln)
			{
				if (ln->type == eventTypeArg && ln->listener == listener && 
					ln->group == groupArg && ln->useCapture == capture)
					return ln;
				ln = ln->next;
			}
			return 0;
		}

		// Event
		void Event::StopPropagation()
		{
			if (cancellable)
				isCancelled;
		}

		void Event::PreventDefault()
		{
		}

		void Event::InitEvent(unsigned int eventTypeArg, bool canBubbleArg, bool cancellableArg)
		{
			eventPhase = 0;
			type = eventTypeArg;
			bubbles = canBubbleArg;
			cancellable = cancellableArg;
		}

		// Event Listener
		void EventListener::HandleEvent(Event *evt)
		{
			// do javascript...
		}
}