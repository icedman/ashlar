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

#include <common.h>
#include <debug.h>
#include <script/jsw.h>
#include <net/xmlrequest.h>

class JSXmlRequest : public JSWBaseClass<JSXmlRequest, Net::XmlRequest>
{
public:

	JSXmlRequest()
	{
		SetPrivate(0, false);
	}

	JSXmlRequest(Net::XmlRequest *p, bool bDelete)
	{
		SetPrivate(p, bDelete);
	}

	static JSBool SetProperty(Net::XmlRequest* xmlreq, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);
	static JSBool GetProperty(Net::XmlRequest* xmlreq, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);

	static JSBool open(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool send(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	enum {
		responseXml
	};
};
