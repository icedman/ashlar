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

#include <common.h>
#include <rect.h>

namespace Ash
{
	//! IWindow abstract class
	class IWindow
	{
	public:
		virtual bool Create(int w, int h) = 0;
		virtual void Destroy() = 0;

		virtual bool SetSize(int x, int y, int w, int h) = 0;
		virtual void Show(bool show) = 0;

		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnSize(const Rect *rc) = 0;
		virtual void OnKeyDown(long key) = 0;
		virtual void OnKeyUp(long key) = 0;
		virtual void OnMouseMove(Point p) = 0;
		virtual void OnMouseLeave() = 0;
		virtual void OnMouseDown(int button, Point p) = 0;
		virtual void OnMouseUp(int button, Point p) = 0;
		virtual void OnDraw(HDC hdc, Rect *rc) = 0;
	};
}