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

Credits:
Franky Braem - wxJS
*/

#pragma once

#include <common.h>
#include <js/src/jsapi.h>

//!  Define a global class
#define JSW_GLOBAL_CLASS(_globalObj) \
	JSClass _globalObj = \
{ \
	"Global", 0, \
	JS_PropertyStub,  JS_PropertyStub,JS_PropertyStub, JS_PropertyStub, \
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,  JS_FinalizeStub  \
};

//!  Function declaration macros
#define JSW_BEGIN_FUNCTION_MAP(_funcs) JSFunctionSpec _funcs[] = {
#define JSW_ADD_FUNCTION(_name, _native, _nargs) { _name, _native, _nargs },
#define JSW_END_FUNCTION_MAP {0} };

//!  Property declaration macros
#define JSW_BEGIN_PROPERTY_MAP(_class) JSPropertySpec _class::properties[] = {
#define JSW_ADD_PROPERTY(_name, _native, _flags) { _name, _native, _flags },
#define JSW_END_PROPERTY_MAP {0} };

//!  Method declaration macros
#define JSW_BEGIN_METHOD_MAP(_class) JSFunctionSpec _class::methods[] = {
#define JSW_ADD_METHOD(_name, _native, _nargs) { _name, _native, _nargs, 0, 0 },
#define JSW_END_METHOD_MAP {0} };

//!  Class name declaration macro
#define JSW_CLASS_NAME(_class, _name) const char* _class::jsClassName = _name;

//!  JSWBaseClass 
/*!
Base class for export objects
*/

template<class T_Proto, class T_Priv>
class JSWBaseClass
{
public:

	//!< constructor
	JSWBaseClass() : pPrivate(NULL), bAutoDelete(true)
	{}

	//!< destructor
	virtual ~JSWBaseClass()
	{
		if (pPrivate && bAutoDelete)
		{
			delete pPrivate;
			pPrivate = NULL;
		}
	}

	//!< get property
	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
	{
		if (JSVAL_IS_INT(id)) 
		{
			T_Proto *p = (T_Proto *) JS_GetPrivate(cx, obj);
			if (p)
			{
				T_Priv *priv = p->GetPrivate();
				return p->GetProperty(priv, JSVAL_TO_INT(id), cx, obj, vp);
			}
		}
		return JS_TRUE;
	}

	//!< set property
	static JSBool JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
	{
		if (JSVAL_IS_INT(id)) 
		{
			T_Proto *p = (T_Proto *) JS_GetPrivate(cx, obj);
			if (p)
			{
				T_Priv *priv = p->GetPrivate();
				return p->SetProperty(priv, JSVAL_TO_INT(id), cx, obj, vp);
			}
		}
		return JS_TRUE;
	}

	//!< constructor function
	static JSBool JSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		T_Proto *priv = new T_Proto();
		priv->SetPrivate(new T_Priv());
		JS_SetPrivate(cx, obj, (void *) priv);
		return JS_TRUE;
	}

	//!< destructor, finalize function
	static void JSDestructor(JSContext *cx, JSObject *obj)
	{
		T_Proto *priv = (T_Proto*) JS_GetPrivate(cx, obj);
		delete priv;
		priv = NULL;
	}

	//!< JSInit
	static JSObject* JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL)
	{
		JSObject *newProtoObj = JS_InitClass(cx, obj, proto, &jswClass, 
			T_Proto::JSConstructor, 0,
			NULL, T_Proto::methods,
			NULL, NULL);
		JS_DefineProperties(cx, newProtoObj, T_Proto::properties);
		return newProtoObj;
	}

	//!< save native class
	void SetPrivate(T_Priv *priv, bool bDelete = true)
	{
		bAutoDelete = bDelete;
		pPrivate = priv; 
	}

	//!< get native class
	T_Priv* GetPrivate() 
	{
		return pPrivate; 
	}

	//!< is private class on auto delete
	bool IsAutoDelete() { return bAutoDelete; }

	//!< set private class to auto delete
	void SetAutoDelete(bool bDelete) { bAutoDelete = bDelete; }

	//!< get class
	static JSClass* GetClass() { return &jswClass; }

	//!< get class name
	static const char* GetClassName() { return jsClassName; }

	//!< Create new object
	static JSObject* Create(JSContext *cx, T_Priv *priv, bool bAutoDelete = true)
	{
		T_Proto *proto = new T_Proto();
		proto->SetPrivate(priv, bAutoDelete);
		JSObject *obj = JS_NewObject(cx, T_Proto::GetClass(), 0, 0);
		JS_SetPrivate(cx, obj, proto);
		return obj;
	}

#if 0
	//!< SetProperty - implement on class
	static JSBool SetProperty(T_Priv* priv, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);

	//!< GetProperty - implement on class
	static JSBool GetProperty(T_Priv* priv, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);
#endif

private:

	T_Priv *pPrivate;							/*!< native class pointer */
	bool bAutoDelete;
	static JSClass jswClass;				/*!< class definition */
	static const char* jsClassName;		/*!< class name */
	static JSPropertySpec properties[];
	static JSFunctionSpec methods[];
};

//!  JSWBaseClass::jswClass
/*!
Base class definition
*/
template<class T_Proto, class T_Priv>
JSClass JSWBaseClass<T_Proto, T_Priv>::jswClass = 
{ 
	JSWBaseClass<T_Proto, T_Priv>::jsClassName,
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,	// JSWBaseClass<T_Proto, T_Priv>::JSAddProperty,
	JS_PropertyStub,
	JSWBaseClass<T_Proto, T_Priv>::JSGetProperty,
	JSWBaseClass<T_Proto, T_Priv>::JSSetProperty,
	JS_EnumerateStub, // (JSEnumerateOp) JSWBaseClass<T_Proto, T_Priv>::JSEnumerate,
	JS_ResolveStub,   // JSWBaseClass<T_Proto, T_Priv>::JSResolve,
	JS_ConvertStub,
	JSWBaseClass<T_Proto, T_Priv>::JSDestructor
};

class JSQueryObject : public JSWBaseClass<JSQueryObject, void*>
{
public:
	static JSBool IsType(const char* name);
};