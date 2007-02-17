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

#include <dom/document.h>
#include <dom/safenode.h>

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

	NodeList2* Element::GetElementsByTagName(DOMString *tagName)
	{
		NodeList2 *n = new NodeList2();
		DOMNode *e = FirstChild();
		while(e)
		{
			if (stricmp(e->nodeName.c_str(), tagName->c_str()) == 0)
			{
				n->push_back(e);
			}
			// deep
			if (1)
			{
				NodeList2 *nn = ((Element *)e)->GetElementsByTagName(tagName);
				for(int i=0; i<nn->Length(); i++)
				{
					n->push_back(nn->Item(i));
				}
				delete nn;
			}
			e = e->NextSibling();
		}
		return n;
	}

	NodeList2* Element::GetElementsById(DOMString *id)
	{
		return GetElementsByAttribute(&DOMString("id"), id);
	}

	NodeList2* Element::GetElementsByAttribute(DOMString *attr, DOMString *value)
	{
		NodeList2 *n = new NodeList2();
		DOMNode *e = FirstChild();
		while(e)
		{
			DOMString *tmp = SafeNode((Element*)e).GetAttribute(attr)->Value();
			if (tmp)
			{
				if (stricmp(tmp->c_str(), value->c_str()) == 0)
				{
					n->push_back(e);
				}
			}
			// deep
			if (1)
			{
				NodeList2 *nn = ((Element *)e)->GetElementsByAttribute(attr, value);
				for(int i=0; i<nn->Length(); i++)
				{
					n->push_back(nn->Item(i));
				}
				delete nn;
			}
			e = e->NextSibling();
		}
		return n;
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
		return attributes.SetNamedItem(node);
	}

	DOMNode* Element::Clone(bool deep)
	{		
		Element *n = new Element();
		n->nodeName = nodeName;
		n->nodeValue = nodeValue;
		n->data = data;

		// attributes
		if (attributes.Length())
		{
			DOMNode *a = attributes.GetFirst();
			while(a)
			{
				((Element*)n)->SetAttribute(&a->nodeName, &a->nodeValue);
				a = a->NextSibling();
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
			n = n->NextSibling();
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
			n = n->NextSibling();
		}
		return true;
	}

	// Attribute
	Attribute::Attribute()
	{
		nodeType = ATTRIBUTE_NODE;
	}

	Attribute::Attribute(DOMString *name, DOMString *value)
	{
		nodeType = ATTRIBUTE_NODE;
		nodeName = *name;
		if (value)
		{
			nodeValue = *value;
		}
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