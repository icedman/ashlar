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

#include "debug.h"
#include "common.h"
#include "domstring.h"
#include "list.h"

using namespace Ash;

#include <vector>

namespace Dom
{
	const unsigned short UNKNOWN_NODE = 0;
	const unsigned short ELEMENT_NODE = 1;
	const unsigned short ATTRIBUTE_NODE = 2;
	const unsigned short TEXT_NODE = 3;
	const unsigned short CDATA_SECTION_NODE = 4;
	const unsigned short ENTITY_REFERENCE_NODE = 5;
	const unsigned short ENTITY_NODE = 6;
	const unsigned short PROCESSING_INSTRUCTION_NODE = 7;
	const unsigned short COMMENT_NODE = 8;
	const unsigned short DOCUMENT_NODE = 9;
	const unsigned short DOCUMENT_TYPE_NODE = 10;
	const unsigned short DOCUMENT_FRAGMENT_NODE = 11;
	const unsigned short NOTATION_NODE = 12;

	class DOMNode;

	//! DOM NodeList class
	class NodeList : public List<DOMNode>
	{
	public:
		unsigned long Length() { return Size(); }
		DOMNode* Item(unsigned long index) { return GetAt(index); }
	};

	class NodeList2: public std::vector<DOMNode*>
	{
	public:
		unsigned long Length() { return size(); }
		DOMNode* Item(unsigned long index) { return at(index); }
	};

	//! DOM NamedNodeMap class
	class NamedNodeMap : public NodeList
	{
	public:
		DOMNode* GetNamedItem(DOMString *nodeName);
		DOMNode* SetNamedItem(DOMNode *node);
		DOMNode* RemoveNamedItem(DOMString *nodeName);
	};

	//! DOM Node class
	class DOMNode : public Node<DOMNode>
	{
	public:
		DOMNode();
		DOMNode(DOMString *name);
		virtual ~DOMNode();

		DOMNode* FirstChild() { return childNodes.GetFirst(); }
		DOMNode* LastChild() { return childNodes.GetLast(); }
		DOMNode* PreviousSibling() { return prev; }
		DOMNode* NextSibling() { return next; }
		DOMNode* ParentNode() { return parentNode; }

		DOMNode* AppendChild(DOMNode *node);
		DOMNode* RemoveChild(DOMNode *node);
		DOMNode* ReplaceChild(DOMNode *newNode, DOMNode *oldNode);
		DOMNode* InsertBefore(DOMNode *node, DOMNode *refNode);

		NodeList* ChildNodes() { return &childNodes; }
		NamedNodeMap* Attributes() { return &attributes; }

		DOMNode* OwnerDocument();

		bool HasChildNodes() { return childNodes.Size()>0; }

		virtual DOMNode* Create() { return new DOMNode(); }
		virtual DOMString* Value() { return &nodeValue; }
		virtual DOMNode* Clone(bool deep = false) { return 0; }
		virtual bool CloneChildren(DOMNode *dst, bool deep = false) { return false; }
		virtual bool Merge(DOMNode *node) { return false; }

		virtual void Free();

#ifdef DEBUG
		void Dump();

		inline int CountNodes(int lastCount = 0)
		{
			lastCount++;
			DOMNode *n = childNodes.GetFirst();
			while(n)
			{
				lastCount = n->CountNodes(lastCount);
				n = n->NextSibling();
			}
			return lastCount;
		}
#endif

	public:
		DOMString nodeName;
		DOMString nodeValue;

		NodeList childNodes;
		NamedNodeMap attributes;

		unsigned short nodeType;

	private:
		DOMNode *parentNode;
	};
}