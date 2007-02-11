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

#include "render.h"
#include <cairo-win32.h>
#include <math.h>

namespace Render
{
	bool RenderEngine::InitBuffer(HDC hdcTarget, const Rect* pRect)
	{
		rect = (*pRect);

		DestroyBuffer();

		hdc = CreateCompatibleDC(hdcTarget);
		hBmp = CreateCompatibleBitmap(hdcTarget, pRect->Width(), pRect->Height());
		hOld = SelectObject(hdc, hBmp);

		surface = cairo_win32_surface_create(hdc);
		cairo = cairo_create(surface);
		return true;
	}

	void RenderEngine::DestroyBuffer()
	{
		if (hdc)
		{
			cairo_destroy(cairo);
			cairo_surface_destroy(surface);
			SelectObject(hdc, hOld);
			DeleteDC(hdc);
			DeleteObject(hBmp);
		}
	}

	void RenderEngine::Clear(long color)
	{
		if (!hdc)
		{
			return;
		}

		HBRUSH hbrush = CreateSolidBrush(color);
		FillRect(hdc, &rect, hbrush);
		DeleteObject(hbrush);
	}

	void RenderEngine::Blit(HDC hdcTarget)
	{
		if (!hdc)
		{
			return;
		}

		BitBlt(hdcTarget, rect.left, rect.top, rect.Width(), rect.Height(), hdc, 0, 0, SRCCOPY);
	}

	bool RenderEngine::Render(Frame *pFrame)
	{
		if (!hdc)
		{
			return false;
		}

		LayoutInfo *li = &pFrame->frameStyle.layout;
		bool draw = true;

		// skip invisible
		draw &= (li->visible && li->display);

		Rect r;
		pFrame->GetRect(&r);
		draw &= (r.Width() > 0 && r.Height() > 0);

		if (draw)
		{
			DrawFrame(pFrame);
			printf("render %s (%d, %d)-(%d, %d)\n", pFrame->GetName(), r.left, r.top, r.right, r.bottom);
		} else {
			printf("skip render %s (%d, %d)-(%d, %d)\n", pFrame->GetName(), r.left, r.top, r.right, r.bottom);
		}

		// render children
		FrameList *frames = pFrame->GetFrames();
		Frame *f = frames->GetFirst();
		while(f)
		{
			Render(f);
			f = f->next;
		}

		return true;
	}

	void RenderEngine::Push()
	{
		cairo_save(cairo);
	}

	void RenderEngine::Pop()
	{
		cairo_fill(cairo);
		if (fill)
		{
			cairo_pattern_destroy (fill);
			fill = 0;
		}

		cairo_restore(cairo);
	}

	long RenderEngine::GetColor(long color, double &r, double &g, double &b)
	{
		r = (double)GetRValue(color) / 255;
		g = (double)GetGValue(color) / 255;
		b = (double)GetBValue(color) / 255;
		return color;
	}

	void RenderEngine::RoundToDevicePixels(const Rect *pRect, double &l, double &t, double &r, double &b)
	{
		// todo: where is round()?

		double x = pRect->left;
		double y = pRect->top;

		// left - top
		cairo_user_to_device(cairo, &x, &y);
		x = floor(x);
		y = floor(y);
		cairo_device_to_user(cairo, &x, &y);
		l = x;
		t = y;

		// right - bottom
		x = pRect->Width();
		y = pRect->Height();
		cairo_user_to_device_distance(cairo, &x, &y);
		x = floor(x);
		y = floor(y);
		cairo_device_to_user_distance(cairo, &x, &y);
		r = l + x;
		b = t + y;
	}

	void RenderEngine::DrawFrame(Frame* f)
	{
		Rect r;
		LayoutInfo *li = &f->frameStyle.layout;
		FrameStyle *fs = &f->frameStyle;
		Gradient *gr = &fs->gradient;

		double x, y, x2, y2;

		f->GetBorderRect(&r);
		RoundToDevicePixels(&r, x, y, x2, y2);

		Push();
		DrawBorder(&fs->border, &fs->borderStyle, x, y, x2, y2, true);
		DrawGradient(&fs->gradient, x, y, x2, y2);
		// DrawImage
		// DrawSvg
		DrawRect(x, y, x2, y2);
		Pop();

		// DrawBorder
		DrawBorder(&fs->border, &fs->borderStyle, x, y, x2, y2, false);
	}

	void RenderEngine::DrawRect(double x, double y, double x2, double y2)
	{
		cairo_move_to(cairo, x, y);
		cairo_line_to(cairo, x2, y);
		cairo_line_to(cairo, x2, y2);
		cairo_line_to(cairo, x, y2);
		cairo_close_path(cairo);
	}

	void RenderEngine::DrawBorder(Borders *br, BorderStyle *bs, double  x, double y, double x2, double y2, bool clip)
	{
		double r, g, b;
		GetColor(bs->color, r, g, b);
		cairo_set_source_rgba(cairo, r, g, b, 1);
		cairo_set_line_width(cairo, GetMaxBorder(*br));

		int lt, rt, rb, lb;
		GetBorders(bs->radius, lt, rt, rb, lb);

		// left top
		cairo_move_to(cairo, x + lt, y);
		cairo_line_to(cairo, x2 - rt , y);
		// right top
		if (rt)
		{
			cairo_curve_to(cairo, x2 - rt , y, x2 - 1, y + 1, x2, y + rt);
		}
		// right bottom
		cairo_line_to(cairo, x2, y2 - rb);
		if (rb)
		{
			cairo_curve_to(cairo, x2, y2 - rb, x2 - 1, y2 - 1, x2 - rb, y2);
		}
		// left bottom
		cairo_line_to(cairo, x + lb, y2);
		if (lb)
		{
			cairo_curve_to(cairo, x + lb, y2, x + 1, y2 - 1, x, y2 - lb);
		}
		if (lt)
		{
			cairo_line_to(cairo, x, y + lt);
			cairo_curve_to(cairo, x, y + lt, x + 1, y + 1, x + lt, y);
		} 

		cairo_close_path(cairo);
		if (clip)
			cairo_clip(cairo);

		cairo_stroke(cairo);
		if (clip)
			cairo_new_path(cairo);
	}

	void RenderEngine::DrawGradient(Gradient *gr, double x, double y, double x2, double y2)
	{
		double w = x2 - x;
		double h = y2 - y;

		if (gr->style != LINEAR || gr->style != RADIAL) 
		{
			double gx = w * ((double)gr->x / 255);
			double gy = h * ((double)gr->y / 255);
			double gx2 = w * ((double)gr->x2 / 255);
			double gy2 = h * ((double)gr->y2 / 255);

			if (gr->style == LINEAR) {
				fill = cairo_pattern_create_linear (x + gx, y + gy, x + gx2, y + gy2);
			} else {
				double radius = w * ((double)gr->radius / 255);
				double radius2 = h * ((double)gr->radius2 / 255);
				fill = cairo_pattern_create_radial (x + gx, y + gy, radius, x + gx2, y + gy2, radius2);
			}

			for(int i = 0; i<gr->colorCount; i++)
			{
				double r, g, b;
				GetColor(gr->colors[i], r, g, b);
				double offset = (double)gr->offsets[i] / 255;
				cairo_pattern_add_color_stop_rgba (fill, offset, r, g, b, 1);
			}
		}

		if (fill)
		{
			cairo_set_source (cairo, fill);
		}
	}
}