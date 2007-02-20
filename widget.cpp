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
#include <dom/stylesheet.h>
#include <dom/docbuilder.h>
#include <layout/button.h>
#include <layout/label.h>
#include <layout/windowframe.h>
#include <layout/framebuilder.h>

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
		Document *document = new Document();
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
		fb.Register(new Label());

		window = (WindowFrame*)fb.Build(document);
		if (!window)
			return 0;

		AddFrame(window);

		// register mouse events
		window->RegisterEvents(window);
		styleSheet.ApplyStyle(element);

		if (1)
		{
			Dump();
			styleSheet.Dump();
			document->Dump();
			resources->Dump();
		}

		if (1)
		{
			JSObject *go = scriptEngine.GetGlobalObject();
			JSContext *cx = scriptEngine.GetContext();

			JSElement::JSInit(cx, go);
			JSNodeList::JSInit(cx, go);
			JSDocument::JSInit(cx, go);
			JSObject *obj = JS_DefineObject(cx, go, "widget", JSDocument::GetClass(), 0, JSPROP_PERMANENT);
			if (obj)
			{
				JSDocument *jsd = new JSDocument();
				jsd->SetPrivate((Document*)element, false);
				JS_SetPrivate(cx, obj, jsd);
			}
		}

		SafeNode snode(document);
		int width = snode.GetElement("window")->GetValue("width")->ValueInt(400);
		int height = snode.GetElement("window")->GetValue("height")->ValueInt(400);
		if (!window->CreateNewWindow(width,height))
			return false;
		window->ShowWindow(true);

		// bug
		for(int i=0;i<4;i++)
			window->Layout();

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