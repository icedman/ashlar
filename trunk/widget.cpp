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
#include <script/jselement.h>
#include <script/jsnodelist.h>

namespace Ash
{

	Widget::Widget()
	{
		resources = ResourceManager::GetInstance();
		scriptEngine.Initialize();
	}

	Widget::~Widget()
	{
		scriptEngine.Shutdown();
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

		Dump();
		document->Dump();
		resources->Dump();

		JSObject *go = scriptEngine.GetGlobalObject();
		JSContext *cx = scriptEngine.GetContext();

		JSElement::JSInit(cx, go);
		JSNodeList::JSInit(cx, go);
		JSObject *obj = JS_DefineObject(cx, go, "widget", &JSElement::jswClass, 0, JSPROP_PERMANENT);
		if (obj)
		{
			JSClass *cls = JS_GetClass(obj);
			JSElement *jse = new JSElement();
			jse->SetPrivate(element, false);
			JS_SetPrivate(cx, obj, jse);
			JSElement *p = (JSElement*) JS_GetPrivate(cx, obj);
		}

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