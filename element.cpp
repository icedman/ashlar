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

#include "document.h"
#include "safenode.h"

namespace Dom
{

	Element::Element()
	{
		nodeType = ELEMENT_NODE;
		data = 0;
	}

	Element::Element(DOMString *tagName)
	{
		nodeType = ELEMENT_NODE;
		nodeName = *tagName;
		data = 0;
	}

	Element::~Element()
	{
	}

	DOMString* Element::GetAttribute(DOMString *name)
	{
		DOMNode *n = GetAttributeNode(name);
		if (n)
			return n->Value();
		return 0;
	}

	DOMNode* Element::GetAttributeNode(DOMString *name)
	{
		DOMNode *n = attributes.GetNamedItem(name);
		return n;
	}

	NodeList* Element::GetElementsByTagName(DOMString *tagName)
	{
		NodeList *n = new NodeList();
		DOMNode *e = FirstChild();
		while(e)
		{
			//if (e->nodeName == *tagName)
			if (stricmp(e->nodeName.c_str(), tagName->c_str()) == 0)
			{
				n->Push(e);
			}
			e = e->NextSibling();
		}
		return n;
	}

	NodeList* Element::GetElementsById(DOMString *id)
	{
		return 0;
	}

	bool Element::HasAttribute(DOMString *name)
	{
		return (GetAttributeNode(name)!=0);
	}

	DOMNode* Element::SetAttribute(DOMString *name, DOMString *value)
	{
		Attribute *attr = new Attribute(name, value);
		return SetAttributeNode(attr);
	}

	DOMNode* Element::SetAttributeNode(DOMNode *node)
	{
		attributes.Push(node);
		return node;
	}

	void Element::Dump()
	{
		int level = 0;
		DOMNode *n = ParentNode();
		while(n)
		{
			n = n->ParentNode();
			level++;
		}

		for(int i = 0; i<level; i++) printf("  ");
		printf("%d %s[%d] %s\n", GetData(), nodeName.c_str(), nodeType, nodeValue.c_str());
		if (HasAttributes())
		{
			DOMNode *n = attributes.GetFirst();
			while(n)
			{
				for(int i = 0; i<level; i++) printf("  ");
				printf(" > %s = %s\n", n->nodeName.c_str(), n->nodeValue.c_str());
				n = n->next;
			}
		}

		n = FirstChild();
		while(n)
		{
			((Element*)n)->Dump();
			n = n->NextSibling();
		}
	}

	void Element::Free()
	{
		DOMNode::Free();
	}

	DOMNode* Element::Clone(bool deep)
	{		
		Element *n = new Element();
		n->nodeName = nodeName;
		n->nodeValue = nodeValue;

		// attributes
		if (attributes.Length())
		{
			DOMNode *a = attributes.GetFirst();
			while(a)
			{
				((Element*)n)->SetAttribute(&a->nodeName, &a->nodeValue);
				a = a->next;
			}
		}

		// child nodes
		if (deep && childNodes.Length())
		{
			CloneChildren(n, deep);
		}

		return (DOMNode*)n;
	}

	bool Element::CloneChildren(DOMNode *dst, bool deep)
	{
		DOMNode *n = childNodes.GetFirst();
		while(n)
		{
			DOMNode *clone = (Element*)n->Clone(deep);
			dst->AppendChild(clone);
			n = n->NextSibling();
		}
		return true;
	}

	bool Element::Merge(DOMNode *src)
	{
		// attributes
		DOMNode *n = src->attributes.GetFirst();
		while(n)
		{
			DOMNode *a = GetAttributeNode(&n->nodeName);
			if (a)
			{
				a->nodeValue = n->nodeValue;
			} else {
				SetAttribute(&n->nodeName, &n->nodeValue);
			}
			n = n->next;
		}

		// child nodes
		SafeNode snode(this);
		n = src->childNodes.GetFirst();
		while(n)
		{
			SafeNode *nl = snode.GetElement(&n->nodeName);
			bool merged = false;
			for(int i = 0; i<nl->Length(); i++)
			{
				Element *ee = nl->Item(i)->Node();
				if (ee->nodeName == n->nodeName)
				{
					// same name
					// same id
					ee->Merge(n);
					merged = true;
					break;
				}
			}
			if (!merged)
			{
				AppendChild(n->Clone(true));
			}
			n = n->next;
		}
		return true;
	}

	// Attribute
	Attribute::Attribute()
	{
		nodeType = ATTRIBUTE_NODE;
		isId = false;
	}

	Attribute::Attribute(DOMString *name, DOMString *value, bool id)
	{
		nodeType = ATTRIBUTE_NODE;
		isId = false;

		nodeName = *name;
		if (value)
		{
			nodeValue = *value;
		}
		isId= id; 
	}

	// TextNode
	TextNode::TextNode()
	{
		nodeType = TEXT_NODE;
		nodeName = DOMString("#text"); 
	}

	// CData Section
	CDataSection::CDataSection()
	{
		nodeType = CDATA_SECTION_NODE;
		nodeName = DOMString("#cdata-section");
	}

	// Comment
	Comment::Comment()
	{
		nodeType = COMMENT_NODE;
		nodeName = DOMString("#comment");
	}

}