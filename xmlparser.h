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

#include "common.h"
#include "domstring.h"
#include <expat/expat.h>

namespace Dom
{
	//! An Xml parser wrapped around expat
	class XmlParser
	{
	public:
		XmlParser();
		~XmlParser();

		virtual bool Initialize();
		virtual void Shutdown();
		virtual bool Parse(const char* data, int len, int isFinal);

		virtual void OnStartElement(const XML_Char *name, const XML_Char **attr);
		virtual void OnEndElement(const XML_Char *name);
		virtual void OnStartCDataSection();
		virtual void OnEndCDataSection();
		virtual void OnCDataSection(DOMString *cdata);
		virtual void OnCharacterData(const XML_Char *data, int len);
		virtual void OnComment(const XML_Char *comment);
		virtual void OnDefault(const XML_Char *data, int len);
		virtual void OnXmlDecl(const XML_Char *version, const XML_Char *encoding, int standalone);

	public:
		XML_Parser parser;

		DOMString cdata;
		bool inCdata;
	};
}
