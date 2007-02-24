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

#include <layout/image.h>
#include <layout/windowframe.h>
#include <layout/stylesheet.h>
#include <render/resources.h>
#include <render/imageRes.h>

using namespace Render;

namespace Layout
{
	bool Image::Layout()
	{
		GetImageXml(frameStyle.bgImage, GetElement());
		
		Render::Resources *rm = Render::Resources::GetInstance();
		Render::ImageRes *rc = (ImageRes*)rm->GetResource(frameStyle.bgImage.imageId);
		if (!rc)
			return false;

		Cairo::RefPtr<Cairo::ImageSurface> surface = rc->GetSurface();
		if (!surface)
			return false;

		int w = surface->get_width();
		int h = surface->get_height();

		frameStyle.layout.width = ISASSIGNED(frameStyle.layout.width) ? frameStyle.layout.width : w;
		frameStyle.layout.height = ISASSIGNED(frameStyle.layout.height) ? frameStyle.layout.height : h;
		return true;
	}

	void Image::Draw(Rasterizer *render)
	{
		LayoutInfo *li = &frameStyle.layout;
		bool draw = true;

		Rect r;
		GetRect(&r);

		// skip invisible
		draw &= (li->visible && li->display);
		draw &= (r.Width() > 0 && r.Height() > 0);

		if (!draw)
			return;

		double x, y, x2, y2;

		GetBorderRect(&r);
		x = r.left;
		y = r.top;
		x2 = x + r.Width();
		y2 = y + r.Height();

		// load image
		GetImageXml(frameStyle.bgImage, GetElement());
		DrawFrame(render, x, y, x2, y2);
	}
}