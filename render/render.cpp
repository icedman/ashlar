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

#include <render/render.h>
#include <render/resources.h>
#include <layout/frametypes.h>
#include <dom/safenode.h>
#include <cairomm/context.h>
#include <color.h>

using namespace Ash;

namespace Render
{
	void GetColor(long color, double &r, double &g, double &b)
	{
		r = (double)GetRed(color) / 255;
		g = (double)GetGreen(color) / 255;
		b = (double)GetBlue(color) / 255;
	}

	void Rasterizer::RoundToDevicePixels(const Rect *pRect, double &l, double &t, double &r, double &b)
	{
		l = pRect->left;
		t = pRect->top;
		r = pRect->right;
		b = pRect->bottom;
		return;

		// todo: where is round()?

		double x = pRect->left;
		double y = pRect->top;

		// left - top
		cr->user_to_device(x, y);
		x = floor(x);
		y = floor(y);
		cr->user_to_device(x, y);
		l = x;
		t = y;

		// right - bottom
		x = pRect->Width();
		y = pRect->Height();
		cr->user_to_device_distance(x, y);
		x = floor(x);
		y = floor(y);
		cr->user_to_device_distance(x, y);
		r = l + x;
		b = t + y;
	}

	bool Rasterizer::SetupBuffer(int width, int height)
	{
		DestroyBuffer();
		img = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, width, height);
		cr = Cairo::Context::create(img);

		// antialiased fonts
		Cairo::FontOptions fntOpts;
		fntOpts.set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
		cr->set_font_options(fntOpts);
		return true;
	}

	void Rasterizer::DestroyBuffer()
	{
		cr.clear();
		img.clear();
	}

	bool Rasterizer::Clear()
	{
		if (!cr)
			return false;

		// is this function is cairo?
		unsigned char *data = img->get_data();
		int size = img->get_width() * img->get_height() * 4;
		memset(data, 0, size);

		return true;
	}

	bool Rasterizer::Clear(double r, double g, double b, double a)
	{
		if (!cr)
			return false;

		cr->save();
		cr->set_source_rgba(r, g, b, a);
		cr->paint_with_alpha(a);
		cr->restore();
		return true;
	}

	bool Rasterizer::PaintBuffer(const Cairo::RefPtr< Cairo::Context > &cx, Rect *rc)
	{
		if (!cr)
			return false;

		cx->save();
		cx->set_source(img, 0, 0);
		cx->rectangle(rc->left, rc->top, rc->Width(), rc->Height());
		cx->fill();
		cx->restore();
		return true;
	}

	bool Rasterizer::DrawTextLine(Font *fn, LayoutInfo *li, const char* text, double  x, double y, double x2, double y2)
	{
		if (!cr || !text)
			return false;

		if (!strlen(text))
			return false;

		Resources *rm = Resources::GetInstance();
		Resource *rc = rm->GetResource(fn->fontId);
		if (!rc)
			return false;

		double tx, ty;

		Cairo::TextExtents extents;
		cr->set_font_size(fn->size);
		cr->select_font_face(rc->GetName()->c_str(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
		cr->get_text_extents(text, extents);

		switch(li->align)
		{
		case ALIGN_LEFT:
			tx = x;
			break;
		case ALIGN_RIGHT:
			tx = x + (x2 - x) - extents.width;
			break;
		default:
			tx = x + ((x2 - x)/2 - (extents.width/2));
		}

		switch(li->verticalAlign)
		{
		case ALIGN_TOP:
			ty = y + extents.height;
			break;
		case ALIGN_BOTTOM:
			ty = y2;
			break;
		default:
			ty = y + ((y2 - y)/2 + (extents.height/2.5));
		}

		double r, g, b;
		GetColor(fn->color, r, g, b);
		cr->set_source_rgb(r, g, b);
		cr->move_to(tx, ty);
		cr->show_text(text);

		return true;
	}

	bool Rasterizer::DrawBorder(Borders *br, BorderStyle *bs, double  x, double y, double x2, double y2)
	{
		if (!cr)
			return false;

		double r, g, b;
		double border = GetMaxBorder(*br);

		GetColor(bs->color, r, g, b);
		cr->set_source_rgb(r, g, b);
		cr->set_line_width(border);

		int lt, rt, rb, lb;
		GetBorders(bs->radius, lt, rt, rb, lb);

		// left top
		cr->move_to(x + lt, y);
		cr->line_to(x2 - rt , y);
		// right top
		if (rt)
		{
			cr->curve_to(x2 - rt , y, x2 - 1, y + 1, x2, y + rt);
		}
		// right bottom
		cr->line_to(x2, y2 - rb);
		if (rb)
		{
			cr->curve_to(x2, y2 - rb, x2 - 1, y2 - 1, x2 - rb, y2);
		}
		// left bottom
		cr->line_to(x + lb, y2);
		if (lb)
		{
			cr->curve_to(x + lb, y2, x + 1, y2 - 1, x, y2 - lb);
		}
		if (lt)
		{
			cr->line_to(x, y + lt);
			cr->curve_to(x, y + lt, x + 1, y + 1, x + lt, y);
		} 
		cr->close_path();

		return true;
	}

	bool Rasterizer::DrawGradient(Gradient *gr, double x, double y, double x2, double y2)
	{
		if (!cr)
			return false;

		double w = x2 - x;
		double h = y2 - y;

		Cairo::RefPtr<Cairo::Pattern> pattern;

		if (gr->colorCount>1 && (gr->style == FILL_GRADIENT_LINEAR || gr->style == FILL_GRADIENT_RADIAL))
		{
			Cairo::RefPtr<Cairo::Gradient> gradient;

			double gx = w * ((double)gr->x / 255);
			double gy = h * ((double)gr->y / 255);
			double gx2 = w * ((double)gr->x2 / 255);
			double gy2 = h * ((double)gr->y2 / 255);

			if (gr->style == FILL_GRADIENT_LINEAR) {
				gradient = Cairo::LinearGradient::create(x + gx, y + gy, x + gx2, y + gy2);
			} else {
				double radius = w * ((double)gr->radius / 255);
				double radius2 = h * ((double)gr->radius2 / 255);
				gradient = Cairo::RadialGradient::create(x + gx, y + gy, radius, x + gx2, y + gy2, radius2);
			}

			for(int i = 0; i<gr->colorCount; i++)
			{
				double r, g, b;
				GetColor(gr->colors[i], r, g, b);
				double offset = (double)gr->offsets[i] / 255;
				gradient->add_color_stop_rgb(offset, r, g, b);
			}

			pattern = gradient;
		} else if (gr->colorCount==1) {
			double r, g, b;
			GetColor(gr->colors[0], r, g, b);
			pattern = Cairo::SolidPattern::create_rgb(r, g, b);
		}

		if (pattern)
		{
			cr->set_source(pattern);
			cr->fill();
		}

		return true;
	}

	bool Rasterizer::GetTextExtents(FrameStyle *fs, const char* text, double &width, double &height)
	{
		if (!cr || !text)
			return false;

		if (!strlen(text))
			return false;

		width = 0;
		height = 0;

		Resources *rm = Resources::GetInstance();
		Resource *rc = rm->GetResource(fs->font.fontId);
		if (!rc)
			return false;

		cr->save();
		Cairo::TextExtents extents;
		cr->set_font_size(fs->font.size);
		cr->select_font_face(rc->GetName()->c_str(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
		cr->get_text_extents(text, extents);
		cr->restore();

		// help!
		width = extents.width + 20;
		height = extents.height + 20;

		return true;
	}
}