#pragma once

#include "frames.h"

namespace Ash
{
	class RenderEngine
	{
	public:
		bool SetTarget(HDC hDc) { hdcTarget = hDc; return true; }
		bool Render(Frame *);
		void DrawRect(HDC hDc, const Rect* pRect, long color);
	private:
		HDC hdcTarget;
	};
}