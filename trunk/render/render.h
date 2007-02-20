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
#include <cairomm/context.h>
#include <cairomm/surface.h>

using namespace Ash;
using namespace Layout;

namespace Render
{
	//! Rendering engine
	class RenderEngine
	{
	public:
		RenderEngine() {}
		~RenderEngine() {}

		bool SetupBuffer(int width, int height);
		bool Render(Frame *frame);
		bool DrawFrame(Frame *frame);
		bool DrawText(Font *fn, LayoutInfo *li, const char* text, double  x, double y, double x2, double y2);
		bool DrawBorder(Borders *br, BorderStyle *bs, double  x, double y, double x2, double y2);
		bool DrawGradient(Gradient *gr, double x, double y, double x2, double y2);
		bool DrawImage(Background *bg, double x, double y, double x2, double y2);

		bool Clear();
		bool PaintBuffer(const Cairo::RefPtr< Cairo::Context > &cx, Rect *rc);
		bool GetTextExtents(FrameStyle *fs, const char* text, double &width, double &height);

	private:

		inline void RoundToDevicePixels(const Rect *pRect, double &l, double &t, double &r, double &b);

		// surface buffer
		Cairo::RefPtr<Cairo::ImageSurface> img;
		Cairo::RefPtr<Cairo::Context> cr;
	};
}
