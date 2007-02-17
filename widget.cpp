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

#include <widget.h>
#include <dom/safenode.h>
#include <dom/styleSheet.h>
#include <layout/button.h>

#include <script/jsw.h>

namespace Ash
{

	static JSBool
		my_add (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble x, y;

		if (!JS_ValueToNumber(cx, argv[0], &x))
			return JS_FALSE;

		if (!JS_ValueToNumber(cx, argv[1], &y))
			return JS_FALSE;

		printf("js:%f %f\n", x, y);
		return JS_NewDoubleValue(cx, x + y, rval);
	}

	JSW_BEGIN_FUNCTION_MAP(my_functions)
		JSW_ADD_FUNCTION("my_add", my_add, 2)
		JSW_END_FUNCTION_MAP

	Widget::Widget()
	{
		resources = ResourceManager::GetInstance();
		scriptEngine.Initialize();
		scriptEngine.RegisterFunctions(my_functions);
	}

	Widget::~Widget()
	{
		Free();
	}

	bool Widget::Load(const char* filename)
	{
		DOMDocument *document = new DOMDocument();
		WindowFrame *window = 0;

		SetElement((Element*)document);
		
		if (!document->LoadFile(filename))
			return false;

		styleSheet.Load(document);
		resources->Load(&styleSheet);

		FrameBuilder fb;
		fb.Register(new Frame());
		fb.Register(new HFrame());
		fb.Register(new VFrame());
		fb.Register(new WindowFrame());
		fb.Register(new Button());

		window = (WindowFrame*)fb.Build(document);
		if (!window)
			return 0;

		AddFrame(window);

		// register mouse events
		window->RegisterEvents(window);

		IWindow *w = (IWindow*)window;
		w->Create(400,300);
		w->Show(true);

		styleSheet.ApplyStyle(element);

		// bug: layout need to be called multiple times when no definite widths and heights are available
		for(int i=0; i<2; i++)
			window->Layout();

		resources->Dump();
		scriptEngine.TestScript();
		return true;
	}

	void Widget::Free()
	{
		if (element)
		{
			delete element;
			element = 0;
		}
		resources->Free();
	}
}