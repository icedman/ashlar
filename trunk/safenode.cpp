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

#include "safenode.h"

namespace Dom
{

	SafeNode::SafeNode()
	{
		element = 0;
		explorer = 0;
		nodes = 0;
	}

	SafeNode::SafeNode(Element *e)
	{
		element = e;
		explorer = 0;
		nodes = 0;
	}

	SafeNode::~SafeNode()
	{
		Free();
		FreeNodes();
	}

	bool SafeNode::Attach(Element *e)
	{
		Free();
		FreeNodes();
		element = e;
		return true;
	}

	bool SafeNode::Attach(NodeList2 *n)
	{
		Free();
		FreeNodes();
		nodes = n;
		return true;
	}

	void SafeNode::Free()
	{
		if (explorer)
		{
			delete explorer;
			explorer = 0;
		}
	}

	void SafeNode::FreeNodes()
	{
		if (nodes)
		{
			delete nodes;
			nodes = 0;
		}
	}

	SafeNode* SafeNode::Item(int index)
	{
		Free();
		explorer = new SafeNode();
		if (nodes)
		{
			if (nodes->Length() > index)
				explorer->Attach((Element*)nodes->Item(index));
		}
		return explorer;
	}

	SafeNode* SafeNode::GetElement(DOMString *tagName)
	{
		Free();
		explorer = new SafeNode();
		if (!element && nodes)
		{
			if (nodes->Length())
				element = (Element*)nodes->Item(0);
		}
		if (element)
		{
			NodeList2 *n = element->GetElementsByTagName(tagName);
			explorer->Attach(n);
		}
		return explorer;
	}

	SafeNode* SafeNode::GetAttribute(DOMString *name)
	{
		Free();
		explorer = new SafeNode();
		if (!element && nodes)
		{
			if (nodes->Length())
				element = (Element*)nodes->Item(0);
		}
		if (element)
		{
			explorer->Attach((Element*)element->GetAttributeNode(name));
		}
		return explorer;
	}

	SafeNode* SafeNode::GetValue(DOMString *name)
	{
		SafeNode* snode = GetAttribute(name);
		if (snode->Value())
			return snode;

		snode = GetElement(name);
		if (snode->Item(0)->Value())
		{
			return snode;
		}

		Free();
		explorer = new SafeNode();
		return explorer;
	}

	Element* SafeNode::Node()
	{
		if (element)
			return element; 
		if (nodes)
		{
			if (nodes->Length())
				return (Element*)nodes->Item(0);
		}
		return 0;
	}

	DOMString* SafeNode::Value(DOMString *defaultValue)
	{
		if (element)
			return element->Value();
		if (nodes)
		{
			if (nodes->Length())
				return nodes->Item(0)->Value();
		}
		return defaultValue;
	}

	int SafeNode::ValueInt(int defaultValue)
	{
		DOMString* val = Value();
		if (!val)
			return defaultValue;
		return atoi(val->c_str());
	}
}