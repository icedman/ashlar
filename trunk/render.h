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
			fill = 0;
			surface = 0;
			hdc = 0;
			hBmp = 0;
		}

		~RenderEngine()
		{
			DestroyBuffer();
		}

		bool InitBuffer(HDC hdc, const Rect *);
		void DestroyBuffer();
		void Clear(long color);
		void Blit(HDC hdc = 0);

		bool Render(Frame *);
		void DrawFrame(Frame *);
		void DrawRect(double x, double y, double x2, double y2);
		void DrawBorder(Borders *br, BorderStyle *bs, double  x, double y, double x2, double y2, bool clip);
		void DrawGradient(Gradient *gr, double x, double y, double x2, double y2);

	private:

		void Push();
		void Pop();
		void RoundToDevicePixels(const Rect *pRect, double &l, double &t, double &r, double &b);
		long GetColor(long color, double &r, double &g, double &b);

		HDC hdc;
		HGDIOBJ hOld;
		HBITMAP hBmp;
		HDC hdcTarget;
		Rect rect;

		cairo_t *cairo;
		cairo_pattern_t *fill;
		cairo_surface_t *surface;
	};
}