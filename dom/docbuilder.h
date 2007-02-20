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

#include <dom/document.h>
#include <dom/xmlparser.h>

namespace Dom
{
	//! DOM Tree builder
	/*!
	Builds a DOM model during parsing of an xml file
	*/
	class DOMBuilder : public XmlParser
	{
	public:
		bool Initialize(Document *d = 0);
		void Shutdown();
		void OnStartElement(const XML_Char *name, const XML_Char **atts);
		void OnEndElement(const XML_Char *name);
		void OnComment(const XML_Char *comment);
		void OnCharacterData(const XML_Char *data, int len);

		Document* GetDocument() { if (!nodeStack.Size()) return 0; return (Document*)nodeStack.GetAt(0); }
		NodeList nodeStack;
	};
}