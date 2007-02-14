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

#include "element.h"

namespace Dom
{
	//! Safe DOM node query class
	class SafeNode
	{
	public:
		SafeNode();
		SafeNode(Element *e);
		~SafeNode();

		bool Attach(Element *e);
		bool Attach(NodeList *n);
		void Free();
		void FreeNodes();

		//! Calls NodeList->Item()
		SafeNode* Item(int index);
		//! Calls NodeList->Length();
		int Length() { if (nodes) return nodes->Length(); return 0; }
		//! Finds a child element. Call GetElementsByTagName()
		SafeNode* GetElement(DOMString *tagName);
		//! Finds an attribute of an element
		SafeNode* GetAttribute(DOMString *name);
		//! Finds an attribute of an element or and child element of the same name
		SafeNode* GetValue(DOMString *name);
		//! Gets the nodeValue of a resulting query
		DOMString* Value();
		//! Gets the integer value of a resulting query
		int ValueInt(int defaultValue);
		//! Gets the DOMNode of a resulting query
		Element* Node();

		SafeNode* GetElement(const char* name) { return GetElement(&DOMString(name)); }
		SafeNode* GetAttribute(const char* name) { return GetAttribute(&DOMString(name)); }
		SafeNode* GetValue(const char* name) { return GetValue(&DOMString(name)); }

	private:
		SafeNode *explorer;
		Element *element;
		NodeList *nodes;
	};

}
