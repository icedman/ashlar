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
#include <dom/mutationevent.h>

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

	DOMNode* Element::AppendChild(DOMNode *node)
	{
		/*
		todo: dom 2 specs says event is fired before insertion
		*/
		Element *newNode = (Element*)DOMNode::AppendChild(node);
		if (newNode)
		{
			MutationEvent me;
			me.InitMutationEvent(NODE_INSERTED, true, true, this, DOMString(""), node->nodeValue, node->nodeName, MUTATION_ADDITION);
			if (!newNode->PropagateEvent(&me))
				return 0;
		}
		return newNode;
	}

	DOMNode* Element::RemoveChild(DOMNode *node)
	{
		Element *oldNode = (Element*)DOMNode::RemoveChild(node);
		if (oldNode)
		{
			MutationEvent me;
			me.InitMutationEvent(NODE_REMOVED, true, true, this, DOMString(""), node->nodeValue, node->nodeName, MUTATION_ADDITION);
			if (!oldNode->PropagateEvent(&me))
				return 0;
		}

		return oldNode;
	}

	DOMNode* Element::InsertBefore(DOMNode *node, DOMNode *refNode)
	{
		return AppendChild(node);

		// bug!
		if (!refNode)
			return 0;

		Element *newNode = (Element*)DOMNode::InsertBefore(node, refNode);
		if (newNode)
		{
			MutationEvent me;
			me.InitMutationEvent(NODE_INSERTED, true, true, this, DOMString(""), node->nodeValue, node->nodeName, MUTATION_ADDITION);
			if (!newNode->PropagateEvent(&me))
				return 0;
		}

		return newNode;
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

	NodeList2* Element::GetElementsByTagName(DOMString *tagName, bool deep)
	{
		if (!deep && nodeType == DOCUMENT_NODE)
			deep = true;

		NodeList2 *n = new NodeList2();
		DOMNode *e = FirstChild();
		while(e)
		{
			if (stricmp(e->nodeName.c_str(), tagName->c_str()) == 0)
			{
				n->push_back(e);
			}
			if (deep)
			{
				NodeList2 *nn = ((Element *)e)->GetElementsByTagName(tagName, true);
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

	NodeList2* Element::GetElementsByAttribute(DOMString *attr, DOMString *value, bool deep)
	{
		if (!deep && nodeType == DOCUMENT_NODE)
			deep = true;

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
			if (deep)
			{
				NodeList2 *nn = ((Element *)e)->GetElementsByAttribute(attr, value, true);
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
		// existing attribute will be replaced (and deleted)
		Attribute *attr = new Attribute(name, value);
		if (SetAttributeNode(attr))
			return attr;

		delete attr;
		return 0;
	}

	DOMNode* Element::SetAttributeNode(DOMNode *node)
	{
		unsigned short mod = MUTATION_ADDITION;

		DOMString prev;
		Attribute *attr = (Attribute*)GetAttributeNode(&node->nodeName);
		if (attr)
		{
			mod = MUTATION_MODIFICATION;
			prev = attr->nodeValue;
		}

		// if node->nodeValue == null (removal)

		MutationEvent me;
		me.InitMutationEvent(ATTRIBUTE_MODIFIED, true, true, this, prev, node->nodeValue, node->nodeName, mod);
		if (!PropagateEvent(&me))
			return 0;

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

	DOMString* Element::SetValue(DOMString val)
	{
		nodeValue = val;
		return &nodeValue;
	}

	bool Element::PropagateEvent(Event *evt)
	{
		EventTarget *et = (EventTarget*)this;
		et->DispatchEvent(evt);

		if (evt->bubbles)
		{
			Element *p = (Element*)ParentNode();
			while(p)
			{
				et = (EventTarget*)p;
				et->DispatchEvent(evt);
				p = (Element*)p->ParentNode();
			}
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

	TextNode::TextNode(DOMString *value)
	{
		nodeType = TEXT_NODE;
		nodeName = DOMString("#text"); 
		nodeValue = *value;
	}

	// CData Section
	CDataSection::CDataSection()
	{
		nodeType = CDATA_SECTION_NODE;
		nodeName = DOMString("#cdata-section");
	}

	CDataSection::CDataSection(DOMString *value)
	{
		nodeType = CDATA_SECTION_NODE;
		nodeName = DOMString("#cdata-section");
		nodeValue = *value;
	}

	// Comment
	Comment::Comment()
	{
		nodeType = COMMENT_NODE;
		nodeName = DOMString("#comment");
	}

	Comment::Comment(DOMString *value)
	{
		nodeType = COMMENT_NODE;
		nodeName = DOMString("#comment");
		nodeValue = *value;
	}

}