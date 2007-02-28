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

#include <script/jsxmlrequest.h>
#include <script/jsdocument.h>

using namespace Net;

JSBool JSXmlRequest::SetProperty(Net::XmlRequest* xmlreq, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
{
	return JS_TRUE;
}

JSBool JSXmlRequest::GetProperty(Net::XmlRequest* xmlreq, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
{
	*vp = BOOLEAN_TO_JSVAL(JS_FALSE);

	switch(id)
	{
	case responseXml:
		{
			JSObject *obj = JSDocument::Create(cx, &xmlreq->responseXML, false);
			*vp = OBJECT_TO_JSVAL(obj);
			break;
		}
	}

	return JS_TRUE;
}

JSBool JSXmlRequest::open(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

	if (argc < 2)
		return JS_TRUE;

	JSXmlRequest *p = (JSXmlRequest*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		XmlRequest *xml = p->GetPrivate();
		JSString *jstr = JS_ValueToString(cx, argv[1]);
		xml->Open(DOMString("GET"), DOMString((char*)JS_GetStringBytes(jstr)));
		*rval = BOOLEAN_TO_JSVAL(JS_TRUE);
	}
	return JS_TRUE;
}

JSBool JSXmlRequest::send(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

	if (argc != 0)
		return JS_TRUE;

	JSXmlRequest *p = (JSXmlRequest*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		XmlRequest *xml = p->GetPrivate();
		JSString *jstr = JS_ValueToString(cx, argv[1]);
		xml->Send();
		*rval = BOOLEAN_TO_JSVAL(JS_TRUE);
	}
	return JS_TRUE;
}


JSW_CLASS_NAME(JSXmlRequest, "XmlHttpRequest")

JSW_BEGIN_PROPERTY_MAP(JSXmlRequest)
JSW_ADD_PROPERTY("responseXml", responseXml, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_END_PROPERTY_MAP

JSW_BEGIN_METHOD_MAP(JSXmlRequest)
JSW_ADD_METHOD("open", open, 3)
JSW_ADD_METHOD("send", send, 0)
JSW_END_METHOD_MAP
