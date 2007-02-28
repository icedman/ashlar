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
#include <dom/event.h>

class JSEvent : public JSWBaseClass<JSEvent, Dom::Event>
{
public:

	JSEvent()
	{
		SetPrivate(0, false);
	}

	JSEvent(Dom::Event *p, bool bDelete)
	{
		SetPrivate(p, bDelete);
	}

	static JSBool SetProperty(Dom::Event* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
	{
		return JS_TRUE;
	}

	static JSBool GetProperty(Dom::Event* element, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp)
	{
		return JS_TRUE;
	}

	TRACE
};