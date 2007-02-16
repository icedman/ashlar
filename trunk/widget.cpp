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

#include "widget.h"
#include "safenode.h"
#include "styleSheet.h"

namespace Ash
{
	Widget::Widget()
	{
		styleSheet = 0;
	}

	Widget::~Widget()
	{
		Free();
	}

	bool Widget::Load(const char* filename)
	{
		DOMDocument *document = 0;
		WindowFrame *window = 0;

		DOMBuilder db;
		db.Initialize();

		FILE *fp = fopen(filename, "r");
		while(!feof(fp))
		{
			char buffer[1024];
			unsigned short len = fread(buffer, 1, 1024, fp);
			int isFinal = feof(fp);
			db.Parse(buffer, len, isFinal);
		}
		fclose(fp);

		document = db.GetDocument();
		db.Shutdown();

		if (!document)
			return false;
	
		SetElement((Element*)document);

		styleSheet = new StyleSheet();
		styleSheet->Load(document);
		//styleSheet->Dump();

		FrameBuilder fb;
		fb.Register(new Frame());
		fb.Register(new HFrame());
		fb.Register(new VFrame());
		fb.Register(new WindowFrame());
		fb.Register(new Button());

		window = (WindowFrame*)fb.Build(document);
		if (!window)
			return 0;

		// register mouse events
		window->RegisterEvents(window);

		IWindow *w = (IWindow*)window;
		w->Create(400,200);
		w->Show(true);

		styleSheet->ApplyStyle(element);
		// bug: layout need to be called multiple times when no definite widths and heights are available
		window->Layout();

		AddFrame(window);
		return true;
	}

	void Widget::Free()
	{
		if (styleSheet)
		{
			delete styleSheet;
			styleSheet = 0;
		}

		if (element)
		{
			delete element;
			element = 0;
		}
	}
}