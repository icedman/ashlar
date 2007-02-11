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
#include "styledom.h"

namespace Dom
{

	Element::Element()
	{
		nodeType = ELEMENT_NODE;
		frame = 0;
	}

	Element::Element(DOMString *tagName)
	{
		nodeType = ELEMENT_NODE;
		nodeName = *tagName;
		frame = 0;
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

	DOMNode* Element::SetAttribute(DOMString *name, DOMString *value, bool isId)
	{
		Attribute *attr = new Attribute(name, value, isId);
		return SetAttributeNode(attr, isId);
	}

	DOMNode* Element::SetAttributeNode(DOMNode *node, bool isId)
	{
		if (node->nodeType == ATTRIBUTE_NODE)
		{
			((Attribute*)node)->isId = isId;
		}
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
		printf("%s[%d] %s\n", nodeName.c_str(), nodeType, nodeValue.c_str());
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

	Frame* Element::Attach(Frame* pFrame)
	{
		// todo: handle this
		if (frame)
			return 0;

		frame = pFrame;
		Layout::GetStyleXml(frame->frameStyle, this);
		return frame;
	}

	void Element::Free()
	{
		if (frame)
		{
			frame->Free();
			frame = 0;
		}
		DOMNode::Free();
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