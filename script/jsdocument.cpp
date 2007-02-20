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

#include <script/jsdocument.h>

JSBool JSDocument::SetProperty(Document* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
{
	return JSElement::SetProperty(element, id, cx, obj, vp);
}

JSBool JSDocument::GetProperty(Document* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
{
	return JSElement::GetProperty(element, id, cx, obj, vp);
}

JSBool JSDocument::createElement(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	if (argc != 1)
		return JS_TRUE;
	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		JSString *jstr = JS_ValueToString(cx, argv[0]);
		Document *e = (Document*)p->GetPrivate();
		Element *c = (Element*)e->createElement(&DOMString(JS_GetStringBytes(jstr)));
		if (!c)
			return JS_TRUE;
		JSObject *obj = JSElement::Create(cx, c, true);
		*rval = OBJECT_TO_JSVAL(obj);
	}
	return JS_TRUE;
}

JSBool JSDocument::createAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	return JS_TRUE;
}

JSBool JSDocument::createTextNode(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	return JS_TRUE;
}

JSBool JSDocument::createComment(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)	
{
	return JS_TRUE;
}

JSBool JSDocument::createCDataSection(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)	
{
	return JS_TRUE;
}


JSW_CLASS_NAME(JSDocument, "Document")

JSW_BEGIN_PROPERTY_MAP(JSDocument)
JSW_ADD_PROPERTY("tagName", JSElement::name, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("nodeName", JSElement::name, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("nodeValue", JSElement::value, JSPROP_ENUMERATE)
JSW_END_PROPERTY_MAP

JSW_BEGIN_METHOD_MAP(JSDocument)
JSW_ADD_METHOD("hasChildNodes", JSElement::hasChildNodes, 0)
JSW_ADD_METHOD("firstChild", JSElement::firstChild, 0)
JSW_ADD_METHOD("lastChild", JSElement::lastChild, 0)
JSW_ADD_METHOD("nextSibling", JSElement::nextSibling, 0)
JSW_ADD_METHOD("previousSibling", JSElement::previousSibling, 0)
JSW_ADD_METHOD("hasAttribute", JSElement::getAttribute, 1)
JSW_ADD_METHOD("getAttribute", JSElement::getAttribute, 1)
JSW_ADD_METHOD("setAttribute", JSElement::setAttribute, 2)
JSW_ADD_METHOD("getElementsByTagName", JSElement::getElementsByTagName, 1)
JSW_ADD_METHOD("getElementsById", JSElement::getElementsById, 1)
JSW_ADD_METHOD("createElement", createElement, 1)
JSW_ADD_METHOD("createAttribute", createAttribute, 1)
JSW_ADD_METHOD("createTextNode", createTextNode, 1)
JSW_ADD_METHOD("createComment", createComment, 1)
JSW_ADD_METHOD("createCDataSection", createCDataSection, 1)
JSW_END_METHOD_MAP

