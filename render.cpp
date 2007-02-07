#include "render.h"

namespace Ash
{
	bool RenderEngine::Render(Frame *pFrame)
	{
		LayoutInfo *li = &pFrame->layoutInfo;
		bool draw = true;

		// skip invisible
		draw &= (li->visible && li->display);

		Rect r;
		pFrame->GetRect(&r);
		draw &= (r.Width() > 0 && r.Height() > 0);

		if (draw)
		{
			DrawRect(hdcTarget, &r, RGB(255,0,0));
			pFrame->GetBorderRect(&r);
			DrawRect(hdcTarget, &r, RGB(0,0,255));
			printf("render %s %d (%d, %d)-(%d, %d)\n", pFrame->GetName(), pFrame, r.left, r.top, r.right, r.bottom);
		} else {
			printf("skip render %s %d (%d, %d)-(%d, %d)\n", pFrame->GetName(), pFrame, r.left, r.top, r.right, r.bottom);
		}

		FrameList *frames = pFrame->GetFrames();
		FrameList::iterator it = frames->begin();
		while(it != frames->end())
		{
			Render(*it);
			it++;
		}

		return true;
	}

	void RenderEngine::DrawRect(HDC hDc, const Rect* pRect, long color)
	{
		LOGBRUSH lb; 
		HGDIOBJ hPen, hPenOld; 

		lb.lbStyle = BS_SOLID; 
		lb.lbColor = color; 
		lb.lbHatch = 0; 

		hPen = ExtCreatePen(PS_SOLID, 1, &lb, 0, NULL); 
		hPenOld = SelectObject(hDc, hPen);

		POINT p;
		MoveToEx(hDc, pRect->left, pRect->top, &p);
		LineTo(hDc, pRect->right, pRect->top);
		LineTo(hDc, pRect->right, pRect->bottom);
		LineTo(hDc, pRect->left, pRect->bottom);
		LineTo(hDc, pRect->left, pRect->top);

		SelectObject(hDc, hPenOld);
		DeleteObject(hPen);
	}
}