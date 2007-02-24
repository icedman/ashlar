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

#include <layout/grid.h>
#include <layout/stylesheet.h>
#include <dom/safenode.h>

namespace Layout
{
	Grid::Grid()
	{
		frameStyle.layout.flex = 1;
	}

	bool Grid::Layout()
	{
		VFrame::Layout();

		// count columns
		int count = 0;
		Frame *row = frames.GetFirst();
		while(row)
		{
			FrameList *f = row->GetFrames();
			if (count < f->Size())
				count = f->Size();
			row = row->next;
		}

		// setup columns
		FrameList *cols = columns.GetFrames();
		while(cols->Size() < count)
		{
			Frame *f = new Frame();
			columns.AddFrame(f);
		}
		while(cols->Size() > count)
		{
			Frame *f = cols->GetFirst();
			if (f)
				delete f;
		}

		// reset
		Frame *c = cols->GetFirst();
		while(c)
		{	
			SetStyleDefaults(c->frameStyle);
			c = c->next;	
		}

		// calculate sizes (get column widths);
		row = frames.GetFirst();
		while(row)
		{
			Frame *f = row->GetFrames()->GetFirst();
			Frame *c = cols->GetFirst();
			while(f && c)
			{
				LayoutInfo *fli = &f->frameStyle.layout;
				LayoutInfo *cli = &c->frameStyle.layout;
				if (cli->rect.Width() < fli->rect.Width())
				{
					cli->rect = f->frameStyle.layout.rect;
					c->frameStyle = f->frameStyle;
				}
				f = f->next;
				c = c->next;
			}
			row = row->next;
		}

		// read columns
		Dom::SafeNode snode(GetElement());
		Dom::SafeNode *nl = snode.GetElement("columns")->GetElement("col");

		c = cols->GetFirst();
		int i = 0;
		while(c)
		{
			LayoutInfo *cli = &c->frameStyle.layout;
			Dom::Element *ncol = nl->Item(i++)->Node();
			Dom::SafeNode scol(ncol);
			int width = scol.GetValue("width")->ValueInt(cli->width);
			GetLayoutXml(*cli, ncol);
			if (cli->flex)
				width = UNASSIGNED;
			cli->width = width;
			c = c->next;
		}

		// layout columns
		row = frames.GetFirst();
		columns.frameStyle = row->frameStyle;
		columns.Layout();

		// place frames to final positions according to columns
		while(row)
		{
			LayoutInfo *rli = &row->frameStyle.layout;
			Frame *f = row->GetFrames()->GetFirst();
			Frame *c = cols->GetFirst();
			while(f && c)
			{
				LayoutInfo *fli = &f->frameStyle.layout;
				LayoutInfo *cli = &c->frameStyle.layout;

				// original width
				int ow = fli->rect.Width();
				int oh = fli->rect.Height();
				if (fli->flex)
				{
					fli->rect.right = fli->rect.left + cli->rect.Width();
				}

				int moveX = cli->rect.left;
				int moveY = fli->rect.top;
				int align = cli->align;
				int valign = cli->verticalAlign;

				// row specified alignment
				//align = ISASSIGNED(rli->align) ? rli->align : align;
				valign = (rli->verticalAlign!=ALIGN_TOP) ? 0 : valign;
				if (fli->flex)
				{
					align = 0;
					valign = 0;
				}

				// alignment
				switch(align)
				{
				case ALIGN_RIGHT:
					moveX += (cli->rect.Width() - ow);
					break;
				case ALIGN_CENTER:
					moveX += ((cli->rect.Width() >> 1) - (ow >> 1));
					break;
				}
				switch(valign)
				{
				case ALIGN_BOTTOM:
					moveY += (cli->rect.Height() - oh);
					break;
				case ALIGN_MIDDLE:
					moveY += ((cli->rect.Height() >> 1) - (oh >> 1));
					break;
				}

				fli->rect.Move(moveX, moveY);

				f = f->next;
				c = c->next;
			}
			row = row->next;
		}
		return true;
	}

	void Grid::Draw(Render::Rasterizer *render)
	{
		VFrame::Draw(render);
	}

	//
	Row::Row()
	{
		frameStyle.layout.flex = 1;
	}
}
