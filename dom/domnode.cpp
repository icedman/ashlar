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

#include <dom/domnode.h>

namespace Dom
{
	// NamedNodeMap
	DOMNode* NamedNodeMap::GetNamedItem(DOMString *nodeName)
	{
		DOMNode *n = GetFirst();
		while(n)
		{
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
		// replace existing item
		DOMNode *n = RemoveNamedItem(&node->nodeName);
		if (n)
			delete n;

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
		Free();
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

	void DOMNode::Free()
	{
		if (parentNode)
		{
			parentNode->RemoveChild(this);
		}

		// free childNodes
		DOMNode *child = 0;
		while(child = FirstChild())
		{
			delete child;
		}

		// free attributes
		DOMNode *attr;
		while(attr = attributes.GetFirst())
		{
			attributes.Remove(attr);
			delete attr;
		}
	}

	void DOMNode::Dump()
	{
		int level = 0;
		DOMNode *n = ParentNode();
		while(n)
		{
			n = n->ParentNode();
			level++;
		}

		for(int i = 0; i<level; i++) printf("  ");
		printf("%s", nodeName.c_str());
		if (attributes.Length())
		{
			DOMNode *n = attributes.GetFirst();
			printf(" > ");
			while(n)
			{
				printf("%s = %s; ", n->nodeName.c_str(), n->nodeValue.c_str());
				n = n->NextSibling();
			}
		}
		if (1 && nodeValue.length())
		{
			printf("\n");
			for(int i = 0; i<level; i++) printf("  ");
			printf(">%s<", nodeValue.c_str());
		}
		printf("\n");

		n = FirstChild();
		while(n)
		{
			n->Dump();
			n = n->NextSibling();
		}
	}
}