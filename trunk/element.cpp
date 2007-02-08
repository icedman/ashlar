

/*!
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

namespace Ash
{

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
		return 0;
	}

	NodeList* Element::GetElementsById(DOMString *tagName)
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

	void Element::Text()
	{
		printf("%s %d\n", nodeName.c_str(), nodeType);
		if (HasAttributes())
		{
			DOMNode *n = attributes.GetFirst();
			while(n)
			{
				printf("   %s %s\n", n->nodeName.c_str(), n->nodeValue.c_str());
				n = n->next;
			}
		}

		DOMNode *n = FirstChild();
		while(n)
		{
			((Element*)n)->Text();
			n = n->NextSibling();
		}
	}

	// Attribute
	Attribute::Attribute()
	{
		nodeType = ATTRIBUTE_NODE;
		isId = false;
	}

	Attribute::Attribute(DOMString *name, DOMString *value, bool id)
	{
		Attribute();
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