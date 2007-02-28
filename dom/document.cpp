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
#include <dom/docbuilder.h>

namespace Dom
{

	Document::Document()
	{
		nodeType = DOCUMENT_NODE;
		nodeName = DOMString("#document");
	}

	Document::Document(DOMString *name)
	{
		nodeType = DOCUMENT_NODE;
		nodeName = DOMString("#document");

		if (name)
			nodeName = *name;
	}

	Attribute* Document::createAttribute(DOMString *name)
	{
		return new Attribute(name);
	}

	CDataSection* Document::createCDataSection(DOMString *data)
	{
		return new CDataSection(data);
	}

	Comment* Document::createComment(DOMString *data)
	{
		return new Comment(data);
	}

	Element* Document::createElement(DOMString *tagName)
	{
		return new Element(tagName);
	}

	TextNode* Document::createTextNode(DOMString *data)
	{
		return new TextNode(data);
	}

	bool Document::LoadFile(const char* filename)
	{
		DOMBuilder db;
		db.Initialize(this);

		FILE *fp = fopen(filename, "r");
		if (!fp)
			return false;

		while(!feof(fp))
		{
			char buffer[1024];
			unsigned short len = fread(buffer, 1, 1024, fp);
			int isFinal = feof(fp);
			db.Parse(buffer, len, isFinal);
		}
		fclose(fp);

		db.Shutdown();

		return true;
	}

	bool Document::LoadXml(const char* data, int len)
	{
		DOMBuilder db;
		db.Initialize(this);
		db.Parse(data, len, true);
		db.Shutdown();
		return true;
	}
}