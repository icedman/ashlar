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

#include <script/jselement.h>
#include <script/jsnodelist.h>

JSBool JSElement::SetProperty(Element* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
{
	DOMString str;
	JSString *jstr;
	switch(id)
	{
	case value:
		jstr = JS_ValueToString(cx, *vp);
		if (jstr)
		{
			element->nodeValue = (char*)JS_GetStringBytes(jstr);
		}
		break;
	}
	return JS_TRUE;
}

JSBool JSElement::GetProperty(Element* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
{
	DOMString str;
	JSString *jstr;
	switch(id)
	{
	case name:
		str  = element->nodeName;
		jstr = JS_NewStringCopyN(cx, str.c_str(), str.size());
		*vp = STRING_TO_JSVAL(jstr);
		break;
	case value:
		str = element->nodeValue;
		jstr = JS_NewStringCopyN(cx, str.c_str(), str.size());
		*vp = STRING_TO_JSVAL(jstr);
		break;
	}
	// constants
	if (id > JSElement::nodeTypes)
	{
		return id - JSElement::nodeTypes;
	}
	return JS_TRUE;
}

JSBool JSElement::hasChildNodes(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		*rval = INT_TO_JSVAL(p->GetPrivate()->HasChildNodes());
	}
	return JS_TRUE;
}

JSBool JSElement::firstChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		Element *c = (Element*)e->FirstChild();
		if (!c)
			return JS_TRUE;
		JSObject *obj = JSElement::Create(cx, c, false);
		*rval = OBJECT_TO_JSVAL(obj);
	}
	return JS_TRUE;
}

JSBool JSElement::lastChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		Element *c = (Element*)e->LastChild();
		if (!c)
			return JS_TRUE;
		JSObject *obj = JSElement::Create(cx, c, false);
		*rval = OBJECT_TO_JSVAL(obj);
	}
	return JS_TRUE;
}

JSBool JSElement::nextSibling(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		Element *c = (Element*)e->NextSibling();
		if (!c)
			return JS_TRUE;
		JSObject *obj = JSElement::Create(cx, c, false);
		*rval = OBJECT_TO_JSVAL(obj);
	}
	return JS_TRUE;
}

JSBool JSElement::previousSibling(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		Element *c = (Element*)e->PreviousSibling();
		if (!c)
			return JS_TRUE;
		JSObject *obj = JSElement::Create(cx, c, false);
		*rval = OBJECT_TO_JSVAL(obj);
	}
	return JS_TRUE;
}

JSBool JSElement::hasAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	if (argc != 1)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		JSString *jstr = JS_ValueToString(cx, argv[0]);
		if (jstr)
		{
			if (e->HasAttribute(&DOMString((char*)JS_GetStringBytes(jstr))))
			{
				*rval = INT_TO_JSVAL(JS_TRUE);
			}
		}
	}
	return JS_TRUE;
}

JSBool JSElement::getAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	if (argc != 1)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		JSString *jstr = JS_ValueToString(cx, argv[0]);
		if (jstr)
		{
			DOMString *s = e->GetAttribute(&DOMString((char*)JS_GetStringBytes(jstr)));
			if (s)
			{
				*rval = STRING_TO_JSVAL(JS_NewStringCopyN(cx, s->c_str(), s->size()));
			}
		}
	}
	return JS_TRUE;
}

JSBool JSElement::setAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	if (argc != 2)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		JSString *name = JS_ValueToString(cx, argv[0]);
		JSString *value = JS_ValueToString(cx, argv[1]);
		if (name && value)
		{
			e->SetAttribute(&DOMString((char*)JS_GetStringBytes(name)), &DOMString((char*)JS_GetStringBytes(value)));
		}
	}
	return JS_TRUE;
}

JSBool JSElement::getElementsByTagName(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	if (argc != 1)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		JSString *jstr = JS_ValueToString(cx, argv[0]);
		NodeList2 *nl = e->GetElementsByTagName(&DOMString((char*)JS_GetStringBytes(jstr)));
		JSObject *obj = JSNodeList::Create(cx, nl, true);
		*rval = OBJECT_TO_JSVAL(obj);
	}
	return JS_TRUE;
}

JSBool JSElement::getElementsById(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	if (argc != 1)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *e = p->GetPrivate();
		JSString *jstr = JS_ValueToString(cx, argv[0]);
		NodeList2 *nl = e->GetElementsById(&DOMString((char*)JS_GetStringBytes(jstr)));
		JSObject *obj = JSNodeList::Create(cx, nl, true);
		*rval = OBJECT_TO_JSVAL(obj);
	}
	return JS_TRUE;
}


JSW_CLASS_NAME(JSElement, "Element")

JSW_BEGIN_PROPERTY_MAP(JSElement)
JSW_ADD_PROPERTY("tagName", name, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("nodeName", name, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("nodeValue", value, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("value", value, JSPROP_ENUMERATE)
JSW_ADD_PROPERTY("UNKNOWN_NODE", JSElement::nodeTypes + UNKNOWN_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("ELEMENT_NODE", JSElement::nodeTypes + ELEMENT_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("ATTRIBUTE_NODE", JSElement::nodeTypes + ATTRIBUTE_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("TEXT_NODE", JSElement::nodeTypes + TEXT_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("CDATA_SECTION_NODE", JSElement::nodeTypes + CDATA_SECTION_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("ENTITY_REFERENCE_NODE", JSElement::nodeTypes + ENTITY_REFERENCE_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("ENTITY_NODE", JSElement::nodeTypes + ENTITY_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("PROCESSING_INSTRUCTION_NODE", JSElement::nodeTypes + PROCESSING_INSTRUCTION_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("COMMENT_NODE", JSElement::nodeTypes + COMMENT_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("DOCUMENT_NODE", JSElement::nodeTypes + DOCUMENT_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("DOCUMENT_TYPE_NODE", JSElement::nodeTypes + DOCUMENT_TYPE_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("DOCUMENT_FRAGMENT_NODE", JSElement::nodeTypes + DOCUMENT_FRAGMENT_NODE, JSPROP_READONLY)
JSW_ADD_PROPERTY("NOTATION_NODE", JSElement::nodeTypes + NOTATION_NODE, JSPROP_READONLY)
JSW_END_PROPERTY_MAP

JSW_BEGIN_METHOD_MAP(JSElement)
JSW_ADD_METHOD("hasChildNodes", hasChildNodes, 0)
JSW_ADD_METHOD("firstChild", firstChild, 0)
JSW_ADD_METHOD("lastChild", lastChild, 0)
JSW_ADD_METHOD("nextSibling", nextSibling, 0)
JSW_ADD_METHOD("previousSibling", previousSibling, 0)
JSW_ADD_METHOD("hasAttribute", getAttribute, 1)
JSW_ADD_METHOD("getAttribute", getAttribute, 1)
JSW_ADD_METHOD("setAttribute", setAttribute, 2)
JSW_ADD_METHOD("getElementsByTagName", getElementsByTagName, 1)
JSW_ADD_METHOD("getElementsById", getElementsById, 1)
JSW_END_METHOD_MAP

JSW_CLASS_NAME(JSAttribute, "Attribute")

JSW_BEGIN_PROPERTY_MAP(JSAttribute)
JSW_ADD_PROPERTY("name", JSElement::name, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("value", JSElement::value, JSPROP_ENUMERATE)
JSW_END_PROPERTY_MAP

JSW_BEGIN_METHOD_MAP(JSAttribute)
JSW_END_METHOD_MAP
