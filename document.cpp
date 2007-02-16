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

namespace Dom
{

	DOMDocument::DOMDocument()
	{
		nodeType = DOCUMENT_NODE;
		nodeName = DOMString("#document");
	}

	DOMDocument::DOMDocument(DOMString *name)
	{
		nodeType = DOCUMENT_NODE;
		nodeName = DOMString("#document");

		if (name)
			nodeName = *name;
	}

	Attribute* DOMDocument::createAttribute(DOMString *name)
	{
		return new Attribute(name);
	}

	CDataSection* DOMDocument::createCDataSection()
	{
		return new CDataSection();
	}

	Comment* DOMDocument::createComment()
	{
		return new Comment();
	}

	Element* DOMDocument::createElement(DOMString *tagName)
	{
		return new Element(tagName);
	}

	TextNode* DOMDocument::createTextNode()
	{
		return new TextNode();
	}
}