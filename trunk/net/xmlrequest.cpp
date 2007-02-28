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

#include <net/xmlrequest.h>
#include <dom/docbuilder.h>

namespace Net
{
	size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
	{
		XmlRequest *xml = (XmlRequest*)userp;
		xml->db.Parse((const char*)buffer, (size*nmemb), 0);
		//xml->responseText.append((char*)buffer, (size * nmemb));
		return (size*nmemb);
	}

	XmlRequest::XmlRequest()
	{
		curl = curl_easy_init();
	}

	XmlRequest::~XmlRequest()
	{
		curl_easy_cleanup(curl);
	}

	void XmlRequest::Open(DOMString method, DOMString urlArg, bool async, DOMString *user, DOMString *password)
	{
		if (!curl)
			return;

		url = urlArg;
		responseText = "";
		responseXML.Free();

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);
	}

	void XmlRequest::SetRequestHeader(DOMString header, DOMString value)
	{
		if (!curl)
			return;
	}

	void XmlRequest::Send()
	{
		if (!curl)
			return;

		db.Initialize(&responseXML);

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		CURLcode res = curl_easy_perform(curl);
	
		db.Parse("\0\0", 2, true);
		db.Shutdown();
	}

	void XmlRequest::Send(DOMString *data)
	{}

	void XmlRequest::Send(Document *data)
	{}

	void XmlRequest::Abort()
	{}

	DOMString* XmlRequest::GetAllResponseHeaders()
	{
		return 0;
	}

	DOMString* XmlRequest::GetResponseHeader(DOMString header)
	{
		return 0;
	}
}