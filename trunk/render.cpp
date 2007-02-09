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

	void RenderEngine::DrawRect(const Rect* pRect, long color)
	{
		cairo_t *cr = cairo;

		double x, y, x2, y2;
		RoundToDevicePixels(pRect, x, y, x2, y2);
		// todo: adjust odd coordinates by 0.5? khtml

		cairo_set_line_width (cr, 1);
		cairo_move_to(cr, x, y);
		cairo_line_to(cr, x2 - 5, y);
		cairo_curve_to(cr, x2 - 5, y, x2, y, x2, y + 5);
		cairo_line_to(cr, x2, y2);
		cairo_line_to(cr, x, y2);
		cairo_close_path(cr);

		cairo_set_source_rgba(cr, 1, 0, 0, 0.5);
		cairo_stroke (cr);
	}

	void RenderEngine::DrawFrame(Frame* f)
	{
		cairo_t *cr = cairo;
		Rect r;
		LayoutInfo *li = &f->frameStyle.layout;
		FrameStyle *fs = &f->frameStyle;

		double x, y, x2, y2;

		// draw border
		f->GetBorderRect(&r);
		RoundToDevicePixels(&r, x, y, x2, y2);

		cairo_set_line_width (cr, fs->border.width);
		/*
		cairo_move_to(cr, x, y);
		cairo_line_to(cr, x2, y);
		cairo_line_to(cr, x2, y2);
		cairo_line_to(cr, x, y2);
		cairo_close_path(cr);

		cairo_set_source_rgba(cr, 0, 0, 1, 1);
		cairo_stroke (cr);
		*/

		cairo_pattern_t *pat;
		pat = cairo_pattern_create_linear (x, y,  x, y2);
		cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
		cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
		cairo_move_to(cr, x, y);
		cairo_line_to(cr, x2, y);
		cairo_line_to(cr, x2, y2);
		cairo_line_to(cr, x, y2);
		cairo_close_path(cr);
		cairo_set_source (cr, pat);
		cairo_fill (cr);
		cairo_pattern_destroy (pat);

		f->GetRect(&r);
		DrawRect(&r, 0);
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

	void RenderEngine::GetColor(long color, double &r, double &g, double &b, double &a)
	{
	}
}