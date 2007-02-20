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

using namespace Dom;

class JSNodeList : public JSWBaseClass<JSNodeList, NodeList2>
{
public:
	static JSBool SetProperty(NodeList2* nodelist, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);
	static JSBool GetProperty(NodeList2* nodelist, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);

	static JSBool item(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool length(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	TRACE
};