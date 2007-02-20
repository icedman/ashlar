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

#include <dom/xmlparser.h>

namespace Dom
{

	// Parser handlers. pass on to userData
	static void XMLCALL StartElementHandler(void *userData, const XML_Char *name, const XML_Char **attr)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnStartElement(name, attr);
	}

	static void XMLCALL EndElementHandler(void *userData, const XML_Char *name)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnEndElement(name);
	}

	static void XMLCALL StartCDataSectionHandler(void *userData)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnStartCDataSection();
	}

	static void XMLCALL EndCDataSectionHandler(void *userData)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnEndCDataSection();
	}

	static void XMLCALL CharacterDataHandler(void *userData, const XML_Char *data, int len)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnCharacterData(data, len);
	}

	static void XMLCALL CommentHandler(void *userData, const XML_Char *comment)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnComment(comment);
	}

	static void XMLCALL DefaultHandler(void *userData, const XML_Char *data, int len)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnDefault(data, len);
	}

	static void XMLCALL XmlDeclHandler(void *userData, const XML_Char *version, const XML_Char *encoding, int standalone)
	{
		XmlParser *p = (XmlParser*)userData;
		p->OnXmlDecl(version, encoding, standalone);
	}

	// XmlParser
	XmlParser::XmlParser()
	{
		parser = 0;
	}

	XmlParser::~XmlParser()
	{
		if (parser)
			Shutdown();
	}

	bool XmlParser::Initialize()
	{
		if (parser)
		{
			XML_ParserReset(parser, 0);
			return true;
		}

		parser = XML_ParserCreate(0);

		// element
		XML_SetElementHandler(parser, StartElementHandler, EndElementHandler);
		// cdata
		XML_SetCdataSectionHandler(parser, StartCDataSectionHandler, EndCDataSectionHandler);
		XML_SetCharacterDataHandler(parser, CharacterDataHandler);
		// comment
		XML_SetCommentHandler(parser, CommentHandler);
		// default
		XML_SetDefaultHandler(parser, DefaultHandler);
		XML_SetXmlDeclHandler(parser, XmlDeclHandler);

		XML_SetUserData(parser, (void*)this);
		return true;
	}

	void XmlParser::Shutdown()
	{
		if (parser)
		{
			XML_ParserFree(parser);
			parser = 0;
			//printf("parser done\n");
		}
	}

	bool XmlParser::Parse(const char* data, int len, int isFinal)
	{
		return XML_Parse(parser, data, len, isFinal);
	}

	void XmlParser::OnStartElement(const XML_Char *name, const XML_Char **attr)
	{
		printf("<%s>\n", name);
	}

	void XmlParser::OnEndElement(const XML_Char *name)
	{
		printf("</%s>\n", name);
	}

	void XmlParser::OnStartCDataSection()
	{}

	void XmlParser::OnEndCDataSection()
	{}

	void XmlParser::OnCDataSection(DOMString *cdata)
	{
		printf("<![CDATA[\n%s\n]]>\n", cdata->c_str());
	}

	void XmlParser::OnCharacterData(const XML_Char *data, int len)
	{}

	void XmlParser::OnComment(const XML_Char *comment)
	{
		printf("<!-- %s -->\n", comment);
	}

	void XmlParser::OnDefault(const XML_Char *data, int len)
	{
		return;

		// skip
		std::string s(data, len);
		printf("%s\n", s.c_str());
	}

	void XmlParser::OnXmlDecl(const XML_Char *version, const XML_Char *encoding, int standalone)
	{
		return;

		// skip
		printf("<?xml version=\"%s\"", version);
		if (encoding)
		{
			printf(" encoding=\"%s\"", encoding);
		}
		if (standalone != -1)
		{
			printf(" standalone=\"%d\"", standalone);
		}
		printf("?>\n");
	}
}