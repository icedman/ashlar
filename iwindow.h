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

#include <windows.h> // HDC

namespace Ash
{
	const unsigned int LEFT_BUTTON	= 1;
	const unsigned int RIGHT_BUTTON	= 2;

	// should be the same with eventtypes.h
	const unsigned int MOUSE_CLICK	= 0;
	const unsigned int MOUSE_DOWN		= 1;
	const unsigned int MOUSE_UP		= 2;
	const unsigned int MOUSE_OVER		= 3;
	const unsigned int MOUSE_MOVE		= 4;
	const unsigned int MOUSE_OUT		= 5;

	//! IWindow abstract class
	class IWindow
	{
	public:
		virtual bool CreateNewWindow(int x, int y, int w, int h, IWindow* parent) = 0;
		virtual void DestroyWindow() = 0;
		virtual bool SetWindowSize(int x, int y, int w, int h) = 0;
		virtual void ShowWindow(bool show) = 0;
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnSize(const Rect *rc) = 0;
		virtual void OnKeyEvent(int eventId, long key) = 0;
		virtual void OnMouseEvent(int eventId, int button, Point p) = 0;
		virtual void OnDraw(HDC hdc, Rect *rc) = 0;
	};
}