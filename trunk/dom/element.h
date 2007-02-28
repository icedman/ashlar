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

#include <dom/domnode.h>
#include <debug.h>

namespace Dom
{
	//! DOM Element class
	class Element : public DOMNode
	{
	public:
		Element();
		Element(DOMString *tagName);

		DOMString* TagName() { return &nodeName; }

		DOMNode* AppendChild(DOMNode *node);
		DOMNode* RemoveChild(DOMNode *node);
		DOMNode* InsertBefore(DOMNode *node, DOMNode *refNode);

		DOMString* GetAttribute(DOMString *name);
		DOMNode* GetAttributeNode(DOMString *name);
		NodeList2* GetElementsByTagName(DOMString *tagName, bool deep = false);
		NodeList2* GetElementsById(DOMString *id);
		NodeList2* GetElementsByAttribute(DOMString *attr, DOMString *value, bool deep = false);
		bool HasAttribute(DOMString *name);
		bool HasAttributes() { return attributes.Length()>0; }
		DOMNode* SetAttribute(DOMString *name, DOMString *value = 0);
		DOMNode* SetAttributeNode(DOMNode *node);

		virtual DOMString* Value() { return &nodeValue; }
		virtual DOMString* SetValue(DOMString val);

		void* SetData(void *d) { data = d; return d; }
		void* GetData() { return data; }

		virtual DOMNode* Clone(bool deep = false);
		virtual bool CloneChildren(DOMNode *dst, bool deep = false);
		virtual bool Merge(DOMNode *node);

		virtual bool PropagateEvent(Event *e);

	private:
		void* data;

	};

	//! DOM Attribute class
	class Attribute : public DOMNode
	{
	public:
		Attribute();
		Attribute(DOMString *name, DOMString *value = 0);
		DOMString* Value() { return &nodeValue; }
	};

	//! DOM Text Node class
	class TextNode : public Element
	{
	public:
		TextNode();
		TextNode(DOMString *value);
		DOMString* Data() { return &nodeValue; }
		unsigned long Length() { return nodeValue.length(); }
		virtual DOMNode* Create() { return new TextNode(); }
		DOMString* Value() { return &nodeValue; }

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
		CDataSection(DOMString *value);
	};

	//! DOM Comment class
	class Comment : public TextNode
	{
	public:
		Comment();
		Comment(DOMString *value);
	};
}