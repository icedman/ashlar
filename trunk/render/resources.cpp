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

#include <render/resources.h>
#include <render/imageRes.h>
#include <dom/safenode.h>

#include <cairo.h>

namespace Render
{
	Resource::Resource(DOMString* n, DOMString* src, int t, int i)
	{
		name = *n;
		type = t;
		id = i;

		// do something with source
	}

	Resource::~Resource()
	{
	}

	void Resource::Dump()
	{
		printf("res: #%d %d %s\n", GetId(), type, name.c_str());
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

	Resource* ResourceManager::AddResource(DOMString *name, DOMString* src, int type)
	{
		Resource *r = GetResource(name);
		if (r)
			return r;

		switch(type)
		{
		case IMAGE_RESOURCE:
			r = new ImageRes(name, src, ++id);
			break;
		default:
			r = new Resource(name, 0, type, ++id);
		}

		if (r)
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
			DOMString *family = snode.GetValue("family")->Value();
			if (family)
			{
				AddResource(family, 0, FONT_RESOURCE);
			}
		}
		delete n;

		// add images
		n = element->GetElementsByTagName(&DOMString("background"));		
		for(int i=0; i<n->Length(); i++)
		{
			SafeNode snode((Element*)n->Item(i));
			DOMString *name = snode.GetValue("name")->Value();
			DOMString *src = snode.GetValue("src")->Value();
			if (src)
			{
				if (!name)
					name = src;
				AddResource(name, src, IMAGE_RESOURCE);
			}
		}
		delete n;
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