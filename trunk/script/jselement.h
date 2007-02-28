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
#include <dom/element.h>

class JSElement : public JSWBaseClass<JSElement, Dom::Element>
{
public:

	JSElement()
	{
		SetPrivate(0, false);
	}

	JSElement(Dom::Element *p, bool bDelete)
	{
		SetPrivate(p, bDelete);
	}

	static JSBool SetProperty(Dom::Element* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);
	static JSBool GetProperty(Dom::Element* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);

	static JSBool appendChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool removeChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool replaceChild(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool insertBefore(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	static JSBool hasChildNodes(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool hasAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool getAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool setAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	static JSBool getElementsByTagName(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool getElementsById(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

public:
	enum {
		name,
		value,
		parent,
		firstChild,
		lastChild,
		nextSibling,
		previousSibling,
		nodeTypes = 100
	};

	TRACE
};

class JSAttribute : public JSWBaseClass<JSAttribute, Dom::Element>
{
public:

	JSAttribute()
	{
		SetPrivate(0, false);
	}

	JSAttribute(Dom::Element *p, bool bDelete)
	{
		SetPrivate(p, bDelete);
	}

	static JSBool SetProperty(Dom::Element* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
	{
		return JSElement::SetProperty(element, id, cx, obj, vp);
	}

	static JSBool GetProperty(Dom::Element* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
	{
		return JSElement::GetProperty(element, id, cx, obj, vp);
	}
};
