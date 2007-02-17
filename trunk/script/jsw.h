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

//!  Includes

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
#define JSW_BEGIN_PROPERTY_MAP(_class) JSPropertySpec _class::m_properties[] = {
#define JSW_ADD_PROPERTY(_name, _native, _flags) { _name, _native, _flags },
#define JSW_END_PROPERTY_MAP {0} };

//!  Method declaration macros
#define JSW_BEGIN_METHOD_MAP(_class) JSFunctionSpec _class::m_methods[] = {
#define JSW_ADD_METHOD(_name, _native, _nargs) { _name, _native, _nargs, 0, 0 },
#define JSW_END_METHOD_MAP {0} };

//!  Class name declaration macro
#define JSW_CLASS_NAME(_class, _name) const char* _class::m_jsClassName = _name;

//!  JSWBaseClass 
/*!
Base class for export objects
*/
template<class T_Port, class T_Priv>
class JSWBaseClass
{
public:

	//!< Constructor
	JSWBaseClass() : m_pPrivate(NULL)
	{
	}

	//!< Constructor
	virtual ~JSWBaseClass()
	{
		delete m_pPrivate;
		m_pPrivate = NULL;
	}

	//!< JSGetProperty
	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
	{
		if (JSVAL_IS_INT(id)) 
		{
			T_Port *p = (T_Port *) JS_GetPrivate(cx, obj);
			if (p)
			{
				T_Priv *priv = p->GetPrivate();
				return p->GetProperty(priv, JSVAL_TO_INT(id), cx, obj, vp);
			}
		}
		return JS_TRUE;
	}

	//!< JSSetProperty
	static JSBool JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
	{
		if (JSVAL_IS_INT(id)) 
		{
			T_Port *p = (T_Port *) JS_GetPrivate(cx, obj);
			if (p)
			{
				T_Priv *priv = p->GetPrivate();
				return p->SetProperty(priv, JSVAL_TO_INT(id), cx, obj, vp);
			}
		}
		return JS_TRUE;
	}

	//!< JSConstructor
	static JSBool JSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		T_Port *priv = new T_Port();
		priv->SetPrivate(new T_Priv());
		JS_SetPrivate(cx, obj, (void *) priv);
		return JS_TRUE;
	}

	//!< JSDestructor
	static void JSDestructor(JSContext *cx, JSObject *obj)
	{
		T_Port *priv = (T_Port*) JS_GetPrivate(cx, obj);
		delete priv;
		priv = NULL;
	}

	//!< JSInit
	static JSObject* JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL)
	{
		JSObject *newProtoObj = JS_InitClass(cx, obj, proto, &m_jswClass, 
			T_Port::JSConstructor, 0,
			NULL, T_Port::m_methods,
			NULL, NULL);
		JS_DefineProperties(cx, newProtoObj, T_Port::m_properties);
		return newProtoObj;
	}

	//!< SetPrivate - save native class
	void SetPrivate(T_Priv *priv)
	{
		m_pPrivate = priv; 
	}

	//!< GetPrivate - get native class
	T_Priv* GetPrivate() 
	{
		return m_pPrivate; 
	}

#if 0
	//!< SetProperty - implement on class
	static JSBool SetProperty(T_Priv* priv, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);

	//!< GetProperty - implement on class
	static JSBool GetProperty(T_Priv* priv, JSInt16 id, JSContext *cx, JSObject *obj, jsval *vp);
#endif

protected:

	T_Priv *m_pPrivate;							/*!< native class pointer */
	static JSClass m_jswClass;					/*!< class definition */
	static const char* m_jsClassName;		/*!< class name */
	static JSPropertySpec m_properties[];
	static JSFunctionSpec m_methods[];
};

//!  JSWBaseClass::m_jswClass
/*!
Base class definition
*/
template<class T_Port, class T_Priv>
JSClass JSWBaseClass<T_Port, T_Priv>::m_jswClass = 
{ 
	JSWBaseClass<T_Port, T_Priv>::m_jsClassName,
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, // JSWBaseClass<T_Port, T_Priv>::JSAddProperty,
	JS_PropertyStub,
	JSWBaseClass<T_Port, T_Priv>::JSGetProperty,
	JSWBaseClass<T_Port, T_Priv>::JSSetProperty,
	JS_EnumerateStub, // (JSEnumerateOp) JSWBaseClass<T_Port, T_Priv>::JSEnumerate,
	JS_ResolveStub,   // JSWBaseClass<T_Port, T_Priv>::JSResolve,
	JS_ConvertStub,
	JSWBaseClass<T_Port, T_Priv>::JSDestructor
};
