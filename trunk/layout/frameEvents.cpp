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
#include <layout/frames.h>
#include <script/jsevent.h>
#include <script/jselement.h>
#include <dom/safenode.h>
#include <dom/eventnames.h>

using namespace Dom;

namespace Layout
{
	/*
	todo: 
	follow DOM Event Model propagation method
	  1. find event target
	  2. dispatch event to listeners
	  3. bubble to ancestors or capture from ancestors
   current method only captures events before reaching EventTargets
	*/

	bool Frame::RegisterEventListeners()
	{
		Frame *f = frames.GetFirst();
		while(f)
		{
			f->RegisterEventListeners();
			f = f->next;
		}
		return true;
	}

	bool Frame::PropagateEvent(Dom::Event *evt)
	{	
		Frame *f = frames.GetFirst();
		while(f)
		{
			f->PropagateEvent(evt);
			f = f->next;
		}

		EventTarget *target = (EventTarget*)GetElement();
		target->DispatchEvent(evt);
		return true;
	}

	bool Frame::PropagateUIEvent(Dom::Event *evt)
	{
		Frame *f = frames.GetFirst();
		while(f)
		{
			f->PropagateUIEvent(evt);
			f = f->next;
		}

		EventTarget *target = (EventTarget*)GetElement();
		target->DispatchEvent(evt);
		return true;
	}

	bool Frame::PropagateMouseEvent(Dom::MouseEvent *evt)
	{
		if (evt->GetEventGroup()!=MOUSE_EVENT)
			return false;

		if (evt->bubbles)
		{
			Frame *f = frames.GetFirst();
			while(f)
			{
				f->PropagateMouseEvent(evt);
				f = f->next;
			}
		}

		Rect r;
		GetRect(&r);
		EventTarget *target = (EventTarget*)GetElement();

		MouseEvent *newEvent = 0;

		// inside
		if (r.Contains(Point(evt->clientX, evt->clientY)))
		{
			switch(evt->type)
			{
			case MOUSE_UP:
				{
					if (mouseState == STATE_PRESSED)
					{
						newEvent = new MouseEvent();
						newEvent->InitEvent(
							MOUSE_CLICK, 
							false,
							evt->cancellable,
							evt->screenX,
							evt->screenY,
							evt->clientX,
							evt->clientY,
							evt->ctrlKey,
							evt->shiftKey,
							evt->altKey,
							evt->metaKey,
							evt->button
							);
					}
					mouseState = STATE_NORMAL;
					target->DispatchEvent(evt);
					break;
				}
			case MOUSE_MOVE:
				{
					if (mouseState == STATE_NORMAL)
					{
						newEvent = new MouseEvent();
						newEvent->InitEvent(
							MOUSE_OVER, 
							false,
							evt->cancellable,
							evt->screenX,
							evt->screenY,
							evt->clientX,
							evt->clientY,
							evt->ctrlKey,
							evt->shiftKey,
							evt->altKey,
							evt->metaKey,
							evt->button
							);
					}
					mouseState = STATE_HOVER;
					target->DispatchEvent(evt);
					break;
				}
			case MOUSE_DOWN:
				{
					mouseState = STATE_PRESSED;
					target->DispatchEvent(evt);
					break;
				}
			case MOUSE_CLICK:
				{
					target->DispatchEvent(evt);
					break;
				}
			case MOUSE_OVER:
				{
					target->DispatchEvent(evt);
					break;
				}
			}

			// outside
		} else {
			switch(evt->type)
			{
			case MOUSE_MOVE:
				{
					if (mouseState != STATE_NORMAL)
					{
						newEvent = new MouseEvent();
						newEvent->InitEvent(
							MOUSE_OUT, 
							false,
							evt->cancellable,
							evt->screenX,
							evt->screenY,
							evt->clientX,
							evt->clientY,
							evt->ctrlKey,
							evt->shiftKey,
							evt->altKey,
							evt->metaKey,
							evt->button
							);
					}
					mouseState = STATE_NORMAL;
					break;
				}
			case MOUSE_OUT:
				{
					mouseState = STATE_NORMAL;
					target->DispatchEvent(evt);
					break;
				}
			}
		}

		if (newEvent)
		{
			PropagateMouseEvent(newEvent);
			delete newEvent;
		}
		return true;
	}

	bool Frame::PropagateKeyEvent(Dom::KeyEvent *evt)
	{
		Frame *f = frames.GetFirst();
		while(f)
		{
			f->PropagateKeyEvent(evt);
			f = f->next;
		}

		EventTarget *target = (EventTarget*)GetElement();
		target->DispatchEvent(evt);
		return true;
	}

	void Frame::HandleEvent(Dom::Event *evt)
	{
		Widget *w = 0;
		if (IsType(WIDGET))
			w = (Widget*)this;

		w = (Widget*)GetParent(WIDGET);
		if (!w)
			return;

		ScriptEngine *s = w->GetScriptEngine();
		if (!s)
			return;

		const Dom::EventNames *eventName = GetEventNames(evt->GetEventGroup(), evt->type);
		if (!eventName)
			return;

		SafeNode snode(GetElement());
		DOMString *script = snode.GetValue(eventName->method)->Value();
		if (script)
		{
			JSObject*  gobj = s->GetGlobalObject();
			JSContext* cx = s->GetContext();
			JSObject* targetObj = s->DefineObject("_tempObj", JSElement::GetClass(), new JSElement(GetElement(), false), false);
			JSObject* eventObj = s->DefineObject("_tempEvent", JSEvent::GetClass(), new JSEvent(evt, false), false);

			char *arguments[1] = { "event" }; 
			jsval argv[1] = { OBJECT_TO_JSVAL(eventObj) };

			JS_CompileFunction(cx, targetObj, eventName->method,
                   1, (const char**)arguments,
                   script->c_str(), script->size(),
                   0, 0);

			jsval rval;
			JS_CallFunctionName(cx, targetObj,eventName->method, 1, argv, &rval);

			JS_GC(cx);
		}
	}
}