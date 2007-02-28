/*
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except compliance with
the License. You may obtaa copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Copyright 2007
MarvSanchez
code.google.com/p/ashlar
*/

#pragma once

#include <common.h>
#include <debug.h>
#include <curl/curl.h>
#include <dom/document.h>
#include <dom/docbuilder.h>
#include <dom/event.h>

using namespace Dom;

namespace Net
{
	class XmlRequest : public EventTarget
	{
	public:
		XmlRequest();
		~XmlRequest();

		void Open(DOMString method, DOMString url, bool async = true, DOMString *user = 0, DOMString *password = 0);
		void SetRequestHeader(DOMString header, DOMString value);
		void Send();
		void Send(DOMString *data);
		void Send(Document *data);
		void Abort();
		DOMString* GetAllResponseHeaders();
		DOMString* GetResponseHeader(DOMString header);

		//attribute EventListener   onreadystatechange;
		unsigned short  readyState;
		DOMString responseText;
		Document responseXML;
		unsigned short status;
		DOMString statusText;

		DOMString url;
		DOMBuilder db;

	private:
		CURL *curl;
	};
}