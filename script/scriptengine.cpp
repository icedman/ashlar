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

#include <script/scriptengine.h>
#include <script/jsw.h>

namespace Script
{

	static JSBool echo (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if (argc)
		{
			JSString *jstr = JS_ValueToString(cx, argv[0]);
			printf("%s", JS_GetStringBytes(jstr));
		}
		printf("\n");
		return JS_TRUE;
	}

	JSW_BEGIN_FUNCTION_MAP(scriptFuncs)
		JSW_ADD_FUNCTION("echo", echo, 1)
		JSW_END_FUNCTION_MAP

		JSClass ScriptEngine::jsGlobalClass =
	{
		"Global", 0,
		JS_PropertyStub,  JS_PropertyStub,JS_PropertyStub, JS_PropertyStub,
		JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,  JS_FinalizeStub 
	};

	ScriptEngine::ScriptEngine()
	{
		jsRt = 0;
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

		jsGlobal = JS_NewObject(jsCx, &jsGlobalClass, 0, 0);
		if (!jsGlobal)
			return false;

		RegisterFunctions(scriptFuncs);

		return JS_InitStandardClasses(jsCx, jsGlobal);
	}

	JSObject * ScriptEngine::RegisterObject(JSClass *jsClass)
	{
		return JS_NewObject(jsCx, jsClass, 0, 0);
	}

	bool ScriptEngine::RegisterFunctions(JSFunctionSpec *funcs)
	{
		return (JS_DefineFunctions(jsCx, jsGlobal, funcs) == JS_TRUE);
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
			/*
			JSString *str = JS_ValueToString(jsCx, rval);
			if (JS_GetStringLength(str) > 0)
			{
				printf("%s\n", (char*)JS_GetStringBytes(str));
			}
			*/
		} else {
			printf("error!\n");
		}
		JS_GC(jsCx);
		return ( res == JS_TRUE);
	}

	bool ScriptEngine::TestScript()
	{
		const char *script = "x = 32;";
		return RunScript(script, 7);
	}
}
