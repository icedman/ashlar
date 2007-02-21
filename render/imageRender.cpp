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

#include <render/render.h>
#include <render/imageRes.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>

using namespace Ash;
using namespace Layout;

namespace Render
{
	// TODO: image position(x,y) when extend is EXTEND_NONE

	bool RenderEngine::DrawImage(Background *bg, double x, double y, double x2, double y2)
	{
		if (!cr)
			return false;

		ResourceManager *rm = ResourceManager::GetInstance();
		ImageRes *rc = (ImageRes*)rm->GetResource(bg->imageId);
		if (!rc)
			return false;

		Cairo::RefPtr<Cairo::ImageSurface> surface = rc->GetSurface();
		if (!surface)
			return false;

		double sw = surface->get_width();
		double sh = surface->get_height();

		double l = bg->slice.left;
		double t = bg->slice.top;
		double r = bg->slice.right;
		double b = bg->slice.bottom;

		// simple stretched image
		if (GetMaxBorder(bg->slice) == 0)
		{
			cr->save();
			cr->rectangle(x, y, x2-x, y2-y);
			cr->translate(x, y);

			switch(bg->extend)
			{
			case EXTEND_REPEAT:
				{
					Cairo::RefPtr<Cairo::SurfacePattern> tmpPattern  = Cairo::SurfacePattern::create(surface);
					tmpPattern->set_extend(Cairo::EXTEND_REPEAT);
					cr->set_source(tmpPattern);

					break;
				}
			case EXTEND_STRETCH:
				{
					cr->scale((x2-x)/ sw, (y2-y)/ sh);
					cr->set_source(surface, 0, 0);
					break;
				}
			case EXTEND_NONE:
				cr->set_source(surface, 0, 0);
			}

			if (bg->extend!=EXTEND_HIDDEN)
				cr->fill();
			cr->restore();
			return true;
		}

		// top left
		if (l && t)
		{
			cr->save();
			cr->rectangle(x, y, l, t);
			cr->translate(x, y);
			cr->set_source(surface, 0, 0);
			cr->fill();
			cr->restore();
		}

		// top right
		if (r && t)
		{
			cr->save();
			cr->rectangle(x2 - r, y, r, t);
			cr->translate(x2 - sw, y);
			cr->set_source(surface, 0, 0);
			cr->fill();
			cr->restore();
		}

		// bottom left
		if (l && b)
		{
			cr->save();
			cr->rectangle(x, y2 - b, l, b);
			cr->translate(x, y2 - sh);
			cr->set_source(surface, 0, 0);
			cr->fill();
			cr->restore();
		}

		// bottom right
		if (l && b)
		{
			cr->save();
			cr->rectangle(x2 - r, y2 - b, r, b);
			cr->translate(x2 - sw, y2 - sh);
			cr->set_source(surface, 0, 0);
			cr->fill();
			cr->restore();
		}

		// top
		if (t)
		{
			Cairo::RefPtr<Cairo::ImageSurface> tmpSurface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, sw-l-r, t);
			Cairo::RefPtr<Cairo::Context> tmpCx = Cairo::Context::create(tmpSurface);

			double tw = tmpSurface->get_width();
			double th = tmpSurface->get_height();

			tmpCx->rectangle(0, 0, tw, th);
			tmpCx->translate(-l, 0);
			tmpCx->set_source(surface, 0, 0);
			tmpCx->fill();

			double ww = x2-x-l-r;

			cr->save();
			cr->rectangle(x + l, y, ww, t);

			switch(bg->sliceExtend.top)
			{
			case EXTEND_REPEAT:
				{
					Cairo::RefPtr<Cairo::SurfacePattern> tmpPattern  = Cairo::SurfacePattern::create(tmpSurface);
					tmpPattern->set_extend(Cairo::EXTEND_REPEAT);
					cr->translate(x + l, y);
					cr->set_source(tmpPattern);
					break;
				} 
			case EXTEND_STRETCH:
				{
					double accw = 2;
					if (ww<=tw)
						accw = 0;
					cr->translate(x + l - accw, y);
					cr->scale(ww/(tw-(accw*2)), 1);
					cr->set_source(tmpSurface, 0, 0);
					break;
				}
			case EXTEND_NONE:
				cr->translate(x + l, y);
				cr->set_source(tmpSurface, 0, 0);
			}

			if (bg->sliceExtend.top!=EXTEND_HIDDEN)
				cr->fill();
			cr->restore();
		}

		// bottom
		if (b)
		{
			Cairo::RefPtr<Cairo::ImageSurface> tmpSurface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, sw-l-r, b);
			Cairo::RefPtr<Cairo::Context> tmpCx = Cairo::Context::create(tmpSurface);

			double tw = tmpSurface->get_width();
			double th = tmpSurface->get_height();

			tmpCx->rectangle(0, 0, tw, th);
			tmpCx->translate(-l, -sh+b);
			tmpCx->set_source(surface, 0, 0);
			tmpCx->fill();

			double ww = x2-x-l-r;

			cr->save();
			cr->rectangle(x + l, y2-b, ww, b);

			switch(bg->sliceExtend.bottom)
			{
			case EXTEND_REPEAT:
				{
					Cairo::RefPtr<Cairo::SurfacePattern> tmpPattern  = Cairo::SurfacePattern::create(tmpSurface);
					tmpPattern->set_extend(Cairo::EXTEND_REPEAT);
					cr->translate(x + l, y2-b);
					cr->set_source(tmpPattern);
					break;
				}
			case EXTEND_STRETCH: 
				{
					double accw = 2;
					if (ww<=tw)
						accw = 0;
					cr->translate(x + l -accw, y2-b);
					cr->scale(ww/(tw-(accw*2)), 1);
					cr->set_source(tmpSurface, 0, 0);
					break;
				}
			case EXTEND_NONE:
				cr->translate(x + l, y2-b);
				cr->set_source(tmpSurface, 0, 0);
			}

			if (bg->sliceExtend.bottom!=EXTEND_HIDDEN)
				cr->fill();
			cr->restore();
		}

		// left
		if (l)
		{
			Cairo::RefPtr<Cairo::ImageSurface> tmpSurface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, l, sh-t-b);
			Cairo::RefPtr<Cairo::Context> tmpCx = Cairo::Context::create(tmpSurface);

			double tw = tmpSurface->get_width();
			double th = tmpSurface->get_height();

			tmpCx->rectangle(0, 0, tw, th);
			tmpCx->translate(0, -t);
			tmpCx->set_source(surface, 0, 0);
			tmpCx->fill();

			double hh = y2-y-t-b;

			cr->save();
			cr->rectangle(x, y+t, l, hh);

			switch(bg->sliceExtend.left)
			{
			case EXTEND_REPEAT:
				{
					Cairo::RefPtr<Cairo::SurfacePattern> tmpPattern  = Cairo::SurfacePattern::create(tmpSurface);
					tmpPattern->set_extend(Cairo::EXTEND_REPEAT);
					cr->translate(x, y+t);
					cr->set_source(tmpPattern);
					break;
				}
			case EXTEND_STRETCH:
				{
					double acch = 2;
					if (hh<=th)
						acch = 0;
					cr->translate(x, y+t-acch);
					cr->scale(1, hh/(th-(acch*2)));
					cr->set_source(tmpSurface, 0, 0);
					break;
				}
			case EXTEND_NONE:
				cr->translate(x, y+t);
				cr->set_source(tmpSurface, 0, 0);
			}

			if (bg->sliceExtend.left!=EXTEND_HIDDEN)
				cr->fill();
			cr->restore();
		}

		// right
		if (r)
		{
			Cairo::RefPtr<Cairo::ImageSurface> tmpSurface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, r, sh-t-b);
			Cairo::RefPtr<Cairo::Context> tmpCx = Cairo::Context::create(tmpSurface);

			double tw = tmpSurface->get_width();
			double th = tmpSurface->get_height();

			tmpCx->rectangle(0, 0, tw, th);
			tmpCx->translate(-sw+r, -t);
			tmpCx->set_source(surface, 0, 0);
			tmpCx->fill();

			double hh = y2-y-t-b;

			cr->save();
			cr->rectangle(x2-r, y+t, r, hh);

			switch(bg->sliceExtend.right)
			{
			case EXTEND_REPEAT:
				{
					Cairo::RefPtr<Cairo::SurfacePattern> tmpPattern  = Cairo::SurfacePattern::create(tmpSurface);
					tmpPattern->set_extend(Cairo::EXTEND_REPEAT);
					cr->translate(x2-r, y+t);
					cr->set_source(tmpPattern);
					break;
				}
			case EXTEND_STRETCH:
				{
					double acch = 2;
					if (hh<=th)
						acch = 0;
					cr->translate(x2-r, y+t-acch);
					cr->scale(1, hh/(th-(acch*2)));
					cr->set_source(tmpSurface, 0, 0);
					break;
				}
			case EXTEND_NONE:
				cr->translate(x2-r, y+t);
				cr->set_source(tmpSurface, 0, 0);
			}

			if (bg->sliceExtend.right!=EXTEND_HIDDEN)
				cr->fill();
			cr->restore();
		}

		// center
		double cw = sw-l-r;
		double ch = sh-t-b;
		if (cw>0 && ch>0)
		{
			Cairo::RefPtr<Cairo::ImageSurface> tmpSurface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, cw, ch);
			Cairo::RefPtr<Cairo::Context> tmpCx = Cairo::Context::create(tmpSurface);

			double tw = tmpSurface->get_width();
			double th = tmpSurface->get_height();

			tmpCx->rectangle(0, 0, tw, th);
			tmpCx->translate(-l, -t);
			tmpCx->set_source(surface, 0, 0);
			tmpCx->fill();

			double ww = x2-x-l-r;
			double hh = y2-y-t-b;

			cr->save();
			cr->rectangle(x+l, y+t, ww, hh);

			switch(bg->extend)
			{
			case EXTEND_REPEAT:
				{
					Cairo::RefPtr<Cairo::SurfacePattern> tmpPattern  = Cairo::SurfacePattern::create(tmpSurface);
					tmpPattern->set_extend(Cairo::EXTEND_REPEAT);
					cr->translate(x+l, y+t);
					cr->set_source(tmpPattern);
					break;
				}
			case EXTEND_STRETCH:
				{
					double accw = 2;
					double acch = 2;
					if (ww<=tw)
						accw = 0;
					if (hh<=th)
						acch = 0;
					cr->translate(x+l-accw, y+t-acch);
					cr->scale(ww/(tw-(accw*2)), hh/(th-(acch*2)));
					cr->set_source(tmpSurface, 0, 0);
					break;
				}
			case EXTEND_NONE:
				cr->translate(x+l, y+t);
				cr->set_source(tmpSurface, 0, 0);
			}

			if (bg->extend!=EXTEND_HIDDEN)
				cr->fill();
			cr->restore();
		}

		return true;
	}
}