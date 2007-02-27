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

namespace Dom
{

	const unsigned int UI_EVENTS = 1;
	const unsigned int MOUSE_EVENTS = 2;
	const unsigned int KEY_EVENTS = 3;
	const unsigned int MUTATION_EVENTS = 4;

#define EVENTGROUP(_group) \
	virtual unsigned int GetEventGroup() { return _group; }

}