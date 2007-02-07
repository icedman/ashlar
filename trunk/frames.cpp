#include "frames.h"

namespace Ash
{
	// Frame
	Frame::Frame() 
	{
		parentFrame = 0;

		// default layout
		ZeroMemory(&layoutInfo, sizeof(LayoutInfo));
		layoutInfo.x = UNASSIGNED;
		layoutInfo.y = UNASSIGNED;
		layoutInfo.width = UNASSIGNED;
		layoutInfo.height = UNASSIGNED;
		layoutInfo.margin = 4;
		layoutInfo.marginLeft = UNASSIGNED;
		layoutInfo.marginRight = UNASSIGNED;
		layoutInfo.marginTop = UNASSIGNED;
		layoutInfo.marginBottom = UNASSIGNED;
		layoutInfo.padding = 4;
		layoutInfo.paddingLeft = 10;
		layoutInfo.paddingRight = UNASSIGNED;
		layoutInfo.paddingTop = UNASSIGNED;
		layoutInfo.paddingBottom = UNASSIGNED;
		layoutInfo.display = true;
		layoutInfo.visible = true;
	}

	Frame::~Frame()
	{
	}

	bool Frame::SetParent(Frame* parent)
	{
		parentFrame = (Frame*)parent;
		return true;
	}

	Frame* Frame::GetParent()
	{
		return parentFrame;
	}

	Frame* Frame::GetRoot()
	{
		if (parentFrame)
		{
			return parentFrame->GetRoot();
		}
		return 0;
	}

	bool Frame::AddFrame(Frame* pFrame)
	{
		frames.push_back(pFrame);
		pFrame->SetParent(this);
		return true;
	}

	bool Frame::RemoveFrame(Frame* pFrame)
	{
		FrameList::iterator it = frames.begin();
		while(it != frames.end()) {
			if (*it == pFrame) {
				frames.erase(it);
				pFrame->SetParent(0);
				break;
			}
			it++;
		}
		return true;
	}

	bool Frame::Layout()
	{
		printf("layout %s\n", GetName());
		return true;
	}

	bool Frame::GetRect(Rect *pRect)
	{
		(*pRect) = layoutInfo.rect;
		return true;
	}

	bool Frame::GetBorderRect(Rect *pRect)
	{
		(*pRect) = layoutInfo.rect;
		int mg = ISASSIGNED(layoutInfo.margin) ? layoutInfo.margin : 0;
		int mgl = ISASSIGNED(layoutInfo.marginLeft) ? layoutInfo.marginLeft : mg;
		int mgr = ISASSIGNED(layoutInfo.marginRight) ? layoutInfo.marginRight : mg;
		int mgt = ISASSIGNED(layoutInfo.marginTop) ? layoutInfo.marginTop : mg;
		int mgb = ISASSIGNED(layoutInfo.marginBottom) ? layoutInfo.marginBottom : mg;
		pRect->left += mgl;
		pRect->top += mgt;
		pRect->right -= mgr;
		pRect->bottom -= mgb;
		return true;
	}

	bool Frame::GetClientRect(Rect *pRect)
	{
		GetBorderRect(pRect);
		int pd = ISASSIGNED(layoutInfo.padding) ? layoutInfo.padding : 0;
		int pdl = ISASSIGNED(layoutInfo.paddingLeft) ? layoutInfo.paddingLeft : pd;
		int pdr = ISASSIGNED(layoutInfo.paddingRight) ? layoutInfo.paddingRight : pd;
		int pdt = ISASSIGNED(layoutInfo.paddingTop) ? layoutInfo.paddingTop : pd;
		int pdb = ISASSIGNED(layoutInfo.paddingBottom) ? layoutInfo.paddingBottom : pd;
		pRect->left += pdl;
		pRect->top += pdt;
		pRect->right -= pdr;
		pRect->bottom -= pdb;
		return true;
	}

	bool Frame::SetRect(const Rect* pRect)
	{
		layoutInfo.x = pRect->left;
		layoutInfo.y = pRect->top;
		layoutInfo.width = pRect->Width();
		layoutInfo.height = pRect->Height();
		layoutInfo.rect = (*pRect);
		return true;
	}
};