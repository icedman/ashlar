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

#include <layout/label.h>
#include <layout/windowframe.h>
#include <dom/safenode.h>

using namespace Dom;

namespace Layout
{
	bool Label::Layout()
	{
		WindowFrame *w = (WindowFrame*)GetParent(WINDOW);
		if (w)
		{
			DOMString *text = GetText();
			if (text)
			{
				Render::RenderEngine *r = w->GetRenderer();
				double width, height;
				if (r->GetTextExtents(&frameStyle, text->c_str(), width, height))
				{
					frameStyle.layout.width = ISASSIGNED(frameStyle.layout.width) ? frameStyle.layout.width : width;
					frameStyle.layout.height = ISASSIGNED(frameStyle.layout.height) ? frameStyle.layout.height : height;
				}
			}
		}

		return HFrame::Layout();
	}
}