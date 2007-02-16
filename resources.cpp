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

#include "resources.h"
#include "safenode.h"

namespace Res
{
	Resource::Resource(DOMString* n, void *d, int t, int i)
	{
		name = *n;
		data = d;
		type = t;
		id = i;
	}

	Resource::~Resource()
	{}

	void Resource::Dump()
	{
		printf("res: #%d %s\n", GetId(), name.c_str());
	}

	// resource manager
	ResourceManager::ResourceManager()
	{
		id = 0;
	}

	ResourceManager::~ResourceManager()
	{
		Free();
	}

	void ResourceManager::Free()
	{
		Resource *r = 0;
		while(r = GetFirst())
		{
			Remove(r);
			delete r;
		}
	}

	Resource* ResourceManager::AddResource(DOMString *name, void *data, int type)
	{
		Resource *r = GetResource(name);
		if (r)
			return r;
		r = new Resource(name, data, type, ++id);
		Push(r);
		return r;
	}

	Resource* ResourceManager::GetResource(int id)
	{
		Resource *r = GetFirst();
		while(r)
		{
			if (r->GetId() == id)
				return r;
			r = r->next;
		}
		return 0;
	}

	Resource* ResourceManager::GetResource(DOMString *name)
	{
		Resource *r = GetFirst();
		while(r)
		{
			if (*name == *r->GetName())
				return r;
			r = r->next;
		}
		return 0;
	}

	bool ResourceManager::Load(Element *element)
	{
		// add fonts
		NodeList2 *n = element->GetElementsByTagName(&DOMString("font"));		
		for(int i=0; i<n->Length(); i++)
		{
			SafeNode snode((Element*)n->Item(i));
			DOMString *v = snode.GetValue("name")->Value();
			if (v)
			{
				AddResource(v, 0, FONT_RESOURCE);
			}
		}
		delete n;

		// add images
		return true;
	}

	ResourceManager* ResourceManager::GetInstance()
	{
		static ResourceManager *r = 0;
		if (!r)
			r = new ResourceManager();
		return r;
	}

	void ResourceManager::Dump()
	{
		Resource *r = GetFirst();
		while(r)
		{
			r->Dump();
			r = r->next;
		}
	}
}