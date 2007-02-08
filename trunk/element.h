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

#include "domnode.h"

namespace Dom
{
	//! DOM Element class
	class Element : public DOMNode
	{
	public:
		Element();
		Element(DOMString *tagName);

		DOMString* TagName() { return &nodeName; }

		DOMString* GetAttribute(DOMString *name);
		DOMNode* GetAttributeNode(DOMString *name);
		NodeList* GetElementsByTagName(DOMString *tagName);
		NodeList* GetElementsById(DOMString *tagName);
		bool HasAttribute(DOMString *name);
		bool HasAttributes() { return attributes.Length()>0; }
		DOMNode* SetAttribute(DOMString *name, DOMString *value = 0, bool isId = false);
		DOMNode* SetAttributeNode(DOMNode *node, bool isId = false);

		void Dump();
	};

	//! DOM Attribute class
	class Attribute : public DOMNode
	{
	public:
		Attribute();
		Attribute(DOMString *name, DOMString *value = 0, bool id = false);

		bool isId;
	};

	//! DOM Text Node class
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

	//! DOM Character Data section class
	class CDataSection : public TextNode
	{
	public:
		CDataSection();
	};

	//! DOM Comment class
	class Comment : public TextNode
	{
	public:
		Comment();
	};
}