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

#include "domnode.h"

namespace Dom
{
	// NamedNodeMap
	DOMNode* NamedNodeMap::GetNamedItem(DOMString *nodeName)
	{
		DOMNode *n = GetFirst();
		while(n)
		{
			//if (n->nodeName == *nodeName)
			if (stricmp(n->nodeName.c_str(), nodeName->c_str()) == 0)
			{
				return n;
			}
			n = n->NextSibling();
		}
		return 0;
	}

	DOMNode* NamedNodeMap::SetNamedItem(DOMNode *node)
	{
		if (Push(node))
			return node;
		return 0;
	}

	DOMNode* NamedNodeMap::RemoveNamedItem(DOMString *nodeName)
	{
		DOMNode *n = GetNamedItem(nodeName);
		if (n)
		{
			Remove(n);
			return n;
		}
		return 0;
	}

	// DOMNode
	DOMNode::DOMNode()
	{
		nodeType = UNKNOWN_NODE;
		parentNode = 0;
	}

	DOMNode::DOMNode(DOMString *name)
	{
		nodeType = UNKNOWN_NODE;
		parentNode = 0;
		nodeName = *name;
	}

	DOMNode::~DOMNode()
	{
	}

	DOMNode* DOMNode::AppendChild(DOMNode *node)
	{
		if (childNodes.Push(node))
		{
			node->parentNode = this;
			return node;
		}
		return 0;
	}

	DOMNode* DOMNode::RemoveChild(DOMNode *node)
	{
		if (childNodes.Remove(node))
		{
			node->parentNode = 0;
			return node;
		}
		return 0;
	}

	DOMNode* DOMNode::ReplaceChild(DOMNode *newNode, DOMNode *oldNode)
	{
		if (childNodes.InsertBefore(newNode, oldNode))
		{
			newNode->parentNode = this;
			return RemoveChild(oldNode);
		}
		return oldNode;
	}

	DOMNode* DOMNode::InsertBefore(DOMNode *node, DOMNode *refNode)
	{
		if (childNodes.InsertBefore(node, refNode))
		{
			node->parentNode = this;
			return node;
		}
		return 0;
	}

	DOMNode* DOMNode::OwnerDocument()
	{
		if (parentNode)
		{
			return parentNode->OwnerDocument();
		}
		return this;
	}

	void DOMNode::FreeNodes(DOMNode *n, bool freeSelf)
	{
		if (n->parentNode)
		{
			n->parentNode->RemoveChild(n);
		}

		DOMNode *child = n->FirstChild();
		while(child)
		{
			FreeNodes(child, true);
			child = n->FirstChild();
		}

		if (freeSelf)
			delete n;
	}
}