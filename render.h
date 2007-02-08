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

#include "frames.h"
#include <cairo.h>

using namespace Ash;
using namespace Layout;

namespace Render
{
	//! Rendering engine
	class RenderEngine
	{
	public:
		RenderEngine()
		{
			cairo = 0;
			surface = 0;
			hdc = 0;
			hBmp = 0;
		}

		~RenderEngine()
		{
			DestroyBuffer();
		}

		bool InitBuffer(HDC hdc, const Rect *pRect);
		void DestroyBuffer();
		void Clear(long color);
		void Blit(HDC hdc = 0);

		bool Render(Frame *);
		void DrawRect(const Rect* pRect, long color);
		void DrawFrame(Frame* pFrame);

		void GetColor(long color, double &r, double &g, double &b, double &a);
		void RoundToDevicePixels(const Rect *pRect, double &l, double &t, double &r, double &b);

	private:
		HDC hdc;
		HGDIOBJ hOld;
		HBITMAP hBmp;
		HDC hdcTarget;
		Rect rect;

		cairo_t *cairo;
		cairo_surface_t *surface;
	};
}