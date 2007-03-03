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
#include <layout/button.h>
#include <layout/label.h>
#include <layout/image.h>
#include <layout/grid.h>
#include <layout/windowframe.h>
#include <layout/framebuilder.h>
#include <layout/stylesheet.h>
#include <dom/docbuilder.h>
#include <dom/mutationevent.h>
#include <dom/safenode.h>

namespace Ash
{

	Widget::Widget()
	{
		resources = Resources::GetInstance();
	}

	Widget::~Widget()
	{
		Free();
	}

	bool Widget::Load(const char* filename)
	{
		// load document
		Document *document = new Document();
		SetElement((Element*)document);

		if (!document->LoadFile(filename))
			return false;

		// load stylesheets & resources
		styleSheet.Load(document);
		resources->Load(document);
		resources->Load(&styleSheet);

		// build frame tree
		BuildFrames(document);
		Restyle();

		// setup script engine
		if (1)
		{
			scriptEngine.Initialize();
			JSObject *go = scriptEngine.GetGlobalObject();
			JSContext *cx = scriptEngine.GetContext();
			JSElement::JSInit(cx, go);
			JSNodeList::JSInit(cx, go);
			JSDocument::JSInit(cx, go);
			JSXmlRequest::JSInit(cx, go);
			scriptEngine.DefineObject("widget", JSDocument::GetClass(), new JSDocument((Document*)element, false));
		}

		NodeList2 *nl = document->GetElementsByTagName(&DOMString("script"));
		for(int i=0; i<nl->Length(); i++)
		{
			Element *e = (Element*)nl->Item(i); 
			DOMString *script = e->Value();
			scriptEngine.RunScript(scriptEngine.GetGlobalObject(), script->c_str(), script->size());
		}
		delete nl;

		if (0)
		{
			Dump();
			styleSheet.Dump();
			document->Dump();
			resources->Dump();
		}

		Initialize();
		return true;
	}

	bool Widget::BuildFrames(Element* doc)
	{
		FrameBuilder fb;
		fb.Register(new Frame());
		fb.Register(new HFrame());
		fb.Register(new VFrame());
		fb.Register(new WindowFrame());
		fb.Register(new Button());
		fb.Register(new Label());
		fb.Register(new Image());
		fb.Register(new Grid());
		fb.Register(new Row());

		fb.Build(this, doc);
		return true;
	}

	void Widget::Free()
	{
		// GetElement()->Dump();

		scriptEngine.Shutdown();
		if (element)
		{
			delete element;
			element = 0;
		}

		resources->Free();
	}


	bool Widget::RegisterEventListeners()
	{
		EventTarget *target = (EventTarget*)GetElement();
		target->AddEventListener(MUTATION_EVENTS, NODE_INSERTED, this, false);
		target->AddEventListener(MUTATION_EVENTS, NODE_REMOVED, this, false);
		return Frame::RegisterEventListeners();
	}

	void Widget::HandleEvent(Dom::Event *evt)
	{
		switch(evt->GetEventGroup())
		{
		case MUTATION_EVENTS:
			{
				MutationEvent *me = (MutationEvent*)evt;
				switch(evt->type)
				{
				case NODE_INSERTED:
					{
						BuildFrames(GetElement());
						Frame *f = (Frame*)((Element*)me->relatedNode)->GetData();
						if (f)
						{
							f->Initialize();
						}
						break;
					}
				}
				break;
			}
		}

		Frame::HandleEvent(evt);
	}
}