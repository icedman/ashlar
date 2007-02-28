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

using namespace Dom;

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
			element->SetValue(DOMString((char*)JS_GetStringBytes(jstr)));
		}
		break;
	}
	return JS_TRUE;
}

JSBool JSElement::GetProperty(Element* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
{
	DOMString str;
	JSString *jstr;

	Element *node = 0;
	*vp = BOOLEAN_TO_JSVAL(JS_FALSE);

	switch(id)
	{
	case name:
		{
			str  = element->nodeName;
			jstr = JS_NewStringCopyN(cx, str.c_str(), str.size());
			*vp = STRING_TO_JSVAL(jstr);
			break;
		}
	case value:
		{
			str = element->nodeValue;
			jstr = JS_NewStringCopyN(cx, str.c_str(), str.size());
			*vp = STRING_TO_JSVAL(jstr);
			break;
		}
	case parent:
		node = (Element*)element->ParentNode();
		break;
	case firstChild:
		node = (Element*)element->FirstChild();
		break;
	case lastChild:
		node = (Element*)element->LastChild();
		break;
	case nextSibling:
		node = (Element*)element->NextSibling();
		break;
	case previousSibling:
		node = (Element*)element->PreviousSibling();
		break;
	}

	// node query
	if (node)
	{
		JSObject *obj = JSElement::Create(cx, node, false);
		*vp = OBJECT_TO_JSVAL(obj);
	}

	// constants
	if (id > JSElement::nodeTypes)
	{
		return id - JSElement::nodeTypes;
	}
	return JS_TRUE;
}

JSBool JSElement::appendChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

	if (argc!=1)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *parentNode = p->GetPrivate();

		if (!JSVAL_IS_OBJECT(argv[0]))
			return JS_TRUE;

		JSObject *arg1 = JSVAL_TO_OBJECT(argv[0]);
		JSQueryObject *query = (JSQueryObject*) JS_GetPrivate(cx, arg1); 
		if (!query->IsType(JSElement::GetClassName()))
			return JS_TRUE;

		JSElement *jselement = (JSElement*)query;
		Element *childNode = jselement->GetPrivate();
		if (parentNode->AppendChild(childNode))
		{
			jselement->SetAutoDelete(false);
			*rval = BOOLEAN_TO_JSVAL(JS_TRUE);
		}
	}

	return JS_TRUE;
}

JSBool JSElement::removeChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

	if (argc!=1)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *parentNode = p->GetPrivate();

		if (!JSVAL_IS_OBJECT(argv[0]))
			return JS_TRUE;

		JSObject *arg1 = JSVAL_TO_OBJECT(argv[0]);
		JSQueryObject *query = (JSQueryObject*) JS_GetPrivate(cx, arg1); 
		if (!query->IsType(JSElement::GetClassName()))
			return JS_TRUE;

		JSElement *jselement = (JSElement*)query;
		Element *childNode = jselement->GetPrivate();
		if (parentNode->RemoveChild(childNode))
		{
			jselement->SetAutoDelete(true);
			*rval = BOOLEAN_TO_JSVAL(JS_TRUE);
		}
	}

	return JS_TRUE;
}

JSBool JSElement::replaceChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

	return JS_TRUE;
}

JSBool JSElement::insertBefore(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

	if (argc!=2)
		return JS_TRUE;

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		Element *parentNode = p->GetPrivate();

		if (!JSVAL_IS_OBJECT(argv[0]) || !JSVAL_IS_OBJECT(argv[1]))
			return JS_TRUE;

		JSObject *arg1 = JSVAL_TO_OBJECT(argv[0]);
		JSObject *arg2 = JSVAL_TO_OBJECT(argv[1]);

		JSQueryObject *query = (JSQueryObject*) JS_GetPrivate(cx, arg1); 
		if (!query->IsType(JSElement::GetClassName()))
			return JS_TRUE;
		JSElement *jselement = (JSElement*)query;

		query = (JSQueryObject*) JS_GetPrivate(cx, arg2); 
		if (!query->IsType(JSElement::GetClassName()))
			return JS_TRUE;
		JSElement *jseRef = (JSElement*)query;

		DOMNode *childNode = jselement->GetPrivate();
		DOMNode *refNode = jseRef->GetPrivate();
		if (parentNode->InsertBefore(childNode, refNode))
		{
			jselement->SetAutoDelete(false);
			*rval = BOOLEAN_TO_JSVAL(JS_TRUE);
		}
	}

	return JS_TRUE;
}

JSBool JSElement::hasChildNodes(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

	JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
	if (p->GetPrivate())
	{
		*rval = INT_TO_JSVAL(p->GetPrivate()->HasChildNodes());
	}
	return JS_TRUE;
}

JSBool JSElement::hasAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

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
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

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
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

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
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

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
	*rval = BOOLEAN_TO_JSVAL(JS_FALSE);

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
JSW_ADD_PROPERTY("parent", parent, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("firstChild", firstChild, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("lastChild", lastChild, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("nextSibling", nextSibling, JSPROP_ENUMERATE | JSPROP_READONLY)
JSW_ADD_PROPERTY("previousSibling", previousSibling, JSPROP_ENUMERATE | JSPROP_READONLY)
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
JSW_ADD_METHOD("appendChild", appendChild, 1)
JSW_ADD_METHOD("removeChild", removeChild, 1)
JSW_ADD_METHOD("replaceChild", replaceChild, 2)
JSW_ADD_METHOD("insertBefore", insertBefore, 2)
JSW_ADD_METHOD("hasChildNodes", hasChildNodes, 0)
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
