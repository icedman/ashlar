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

#include <script/jselement.h>
#include <dom/document.h>
\

class JSDocument : public JSWBaseClass<JSDocument, Dom::Document>
{
public:

	JSDocument()
	{
		SetPrivate(0, false);
	}

	JSDocument(Dom::Document *p, bool bDelete)
	{
		SetPrivate(p, bDelete);
	}

	static JSBool SetProperty(Dom::Document* document, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);
	static JSBool GetProperty(Dom::Document* document, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);

	static JSBool createElement(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool createAttribute(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool createTextNode(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool createComment(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool createCDataSection(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	TRACE
};
