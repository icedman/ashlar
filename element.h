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

#pragma once

#include "domnode.h"

namespace Ash
{
	class Element : public DOMNode
	{
	public:
		Element() { nodeType = ELEMENT_NODE; }
		Element(DOMString *tagName) { Element(); nodeName = *tagName; }

		DOMString* tagName() { return &nodeName; }

		DOMString* GetAttribute(DOMString *name);
		DOMNode* GetAttributeNode(DOMString *name);
		NodeList* GetElementsByTagName(DOMString *tagName);
		NodeList* GetElementsById(DOMString *tagName);
		bool HasAttribute(DOMString *name);
		bool HasAttributes() { return attributes.Length()>0; }
		DOMNode* SetAttribute(DOMString *name, DOMString *value = 0, bool isId = false);
		DOMNode* SetAttributeNode(DOMNode *node, bool isId = false);

		virtual void Text();
	};

	class Attribute : public DOMNode
	{
	public:
		Attribute();
		Attribute(DOMString *name, DOMString *value = 0, bool id = false);

		bool isId;
	};

	class TextNode : public Element
	{
	public:
		TextNode();
		DOMString* Data() { return &nodeValue; }
		unsigned long Length() { return nodeValue.length(); }
		virtual DOMNode* Create() { return new TextNode(); }

		// appendData
		// replaceData
		// insertData
		// replaceData
		// splitText
		// substringData
	};

	class CDataSection : public TextNode
	{
	public:
		CDataSection();
	};

	class Comment : public TextNode
	{
	public:
		Comment();
	};
}