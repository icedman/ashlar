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

#include "scriptengine.h"
#include <string.h>

namespace Script
{
	JSClass ScriptEngine::global =
	{
		"Global", 0,
		JS_PropertyStub,  JS_PropertyStub,JS_PropertyStub, JS_PropertyStub,
		JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,  JS_FinalizeStub 
	};

	ScriptEngine::ScriptEngine()
	{
		jsRt = 0;
		jsCx = 0;
	}

	ScriptEngine::~ScriptEngine()
	{
		Shutdown();
	}

	bool ScriptEngine::Initialize()
	{
		jsRt = JS_Init(1000000L);
		if (!jsRt)
			return false;

		jsCx = JS_NewContext(jsRt, 8192);
		if (!jsCx)
			return false;

		jsGlobal = JS_NewObject(jsCx, &global, 0, 0);
		if (!jsGlobal)
			return false;

		return JS_InitStandardClasses(jsCx, jsGlobal);
	}

	void ScriptEngine::Shutdown()
	{
		if (jsCx)
		{
			JS_DestroyContext(jsCx);
			jsCx = 0;
		}
		if (jsRt)
		{
			JS_Finish(jsRt);
			jsRt = 0;
		}
	}

	bool ScriptEngine::RunScript(const char *script, long len)
	{
		if (!jsCx)
			return false;

		jsval rval;
		JSBool res = JS_EvaluateScript(jsCx, jsGlobal, script, len, NULL, 0, &rval);
		if (res == JS_TRUE)
		{
			printf("js:%d\n", JSVAL_TO_INT(rval));
		}
		return ( res == JS_TRUE);
	}

	bool ScriptEngine::TestScript()
	{
		const char *script = "x = 123; x*=2; if (x>0) x=4; for(j=0;j<4;j++) x*=2; x++";
		return RunScript(script, strlen(script));
	}
}
