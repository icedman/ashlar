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

#include <list.h>

namespace Dom
{
	const unsigned short CAPTURING_PHASE = 1;
	const unsigned short AT_TARGET = 2;
	const unsigned short BUBBLING_PHASE = 3;

	class Event;
	class EventTarget;
	class EventListener;

	class EventListenerNode : public Ash::Node<EventListenerNode>
	{
	public:
		EventListenerNode(unsigned int type, EventListener *listener, bool useCapture);
		unsigned int type;
		bool useCapture;
		EventListener *listener;
	};

	class EventListenerNodeList : public Ash::List<EventListenerNode>
	{
	public:
		unsigned long Length() { return Size(); }
		EventListenerNode* Item(unsigned long index) { return GetAt(index); }
		EventListenerNode* Find(unsigned int eventTypeArg, EventListener *listener, bool useCapture);
	};

	class EventTarget
	{
	public:
		virtual ~EventTarget();
		void AddEventListener(unsigned int eventTypeArg, EventListener *listener, bool useCapture);
		void RemoveEventListener(unsigned int eventTypeArg, EventListener *listener, bool useCapture);
		bool DispatchEvent(Event *evt);

		virtual void Free();

	private:
		EventListenerNodeList listeners; 
	};

	class Event
	{
	public:
		void StopPropagation();
		void PreventDefault();
		void InitEvent(unsigned int eventTypeArg, bool canBubbleArg, bool cancelableArg);

		unsigned int type;
		EventTarget target;
		EventTarget currentTarget;
		unsigned short eventPhase;
		bool bubbles;
		bool cancelable;
	};

	class EventListener
	{
	public:
		void HandleEvent(Event *evt);
	};
}