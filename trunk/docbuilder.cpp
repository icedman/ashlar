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

#include "docbuilder.h"

namespace Dom
{
	bool DOMBuilder::Initialize()
	{
		if (!XmlParser::Initialize())
			return false;

		DOMDocument *d = new DOMDocument();
		nodeStack.Clear();
		nodeStack.Push(d);
		return true;
	}

	void DOMBuilder::Shutdown()
	{
		XmlParser::Shutdown();
		nodeStack.Clear();
	}

	void DOMBuilder::OnStartElement(const XML_Char *name, const XML_Char **attr)
	{
		DOMNode *node = nodeStack.GetLast();
		Element *child = new Element(&DOMString(name));
		node->AppendChild(child);
		nodeStack.Push(child);

		// attributes
		for (int i = 0; attr[i]; i += 2) {
			bool isId = strcmp(attr[i], "id") == 0;
			child->SetAttribute(&DOMString(attr[i]), &DOMString(attr[i+1]), isId);
		}
	}

	void DOMBuilder::OnEndElement(const XML_Char *name)
	{
		nodeStack.Pop();
	}

	void DOMBuilder::OnComment(const XML_Char *comment)
	{
		DOMNode *node = nodeStack.GetLast();
		Comment *child = new Comment();
		child->nodeValue = comment;
		node->AppendChild(child);
	}

	void DOMBuilder::OnCharacterData(const XML_Char *data, int len)
	{
		DOMNode *node = nodeStack.GetLast();
		node->nodeValue.append(data, len);
	}
}