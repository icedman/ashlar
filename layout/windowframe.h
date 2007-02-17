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

#include <layout/frames.h>
#include "window.h"
#include <layout/layout.h>
#include <render/render.h>
#include <layout/frametypes.h>

namespace Layout
{
	class WindowFrame : public VFrame, public Ash::Window
	{
	public:
		WindowFrame();
		virtual ~WindowFrame() {}

		const char* GetName() { return "window"; }
		Frame* Create() { return new WindowFrame(); }
		FRAMETYPE(WINDOW, VFrame)

		void OnSize(const Rect *r);
		void OnDraw(HDC hdc, Rect *rc);
		void OnMouseMove(Point p);
		void OnMouseDown(int button, Point p);
		void OnMouseUp(int button, Point p);

		bool RegisterEvents(Frame *frame);
		void Redraw();

	public:
		Render::RenderEngine render;
		Events::MouseEvents mouseEvents;
	};
}