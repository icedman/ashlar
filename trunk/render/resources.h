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
#include <dom/document.h>

using namespace Ash;
using namespace Dom;

namespace Render
{

	const unsigned short RESOURCE_FONT	= 1;
	const unsigned short RESOURCE_IMAGE	= 2;

	class Resource : public Node<Resource>
	{
	public:
		Resource() {}
		Resource(DOMString* name, DOMString* src, int t, int i);
		~Resource();

		inline const DOMString* GetName() { return &name; }
		inline int GetType() { return type; }
		inline int GetId() { return id; }

		void Dump();

	protected:
		DOMString name;
		int type;
		int id;

		TRACE
	};

	class Resources : public List<Resource>
	{
	public:
		Resources();
		~Resources();

		bool Load(Element *element);
		Resource* AddResource(DOMString *name, DOMString* src, int type);
		Resource* GetResource(int id);
		Resource* GetResource(DOMString *name);

		static Resources* GetInstance();

		void Dump();
		void Free();

	public:
		unsigned long id;

	};
}