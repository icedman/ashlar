/*
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except compliance with
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

#include <dom/domnode.h>
#include <dom/event.h>
#include <dom/eventtypes.h>

namespace Dom
{
	const unsigned short MUTATION_ADDITION = 1;
	const unsigned short MUTATION_MODIFICATION = 2;
	const unsigned short MUTATION_REMOVAL = 3;

	const unsigned short SUBTREE_MODIFIED = 0;
	const unsigned short NODE_INSERTED		= 1;
	const unsigned short NODE_REMOVED		= 2;
	const unsigned short NODE_INSERTED_TO_DOCUMENT = 3;
	const unsigned short NODE_REMOVED_FROM_DOCUMENT = 4;
	const unsigned short ATTRIBUTE_MODIFIED = 5;
	const unsigned short CHARACTER_DATA_MODIFIED = 6;

	class MutationEvent : public Event
	{
	public:
		EVENTGROUP(MUTATION_EVENTS)

			void InitMutationEvent(unsigned int eventTypeArg, 
			bool canBubbleArg, 
			bool cancellableArg, 
			DOMNode *relatedNodeArg, 
			DOMString prevValueArg, 
			DOMString newValueArg, 
			DOMString attrNameArg, 
			unsigned short attrChangeArg);

		DOMNode *relatedNode;
		DOMString prevValue; 
		DOMString newValue; 
		DOMString attrName;
		unsigned short attrChange;
	};
}