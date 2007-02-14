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

namespace Ash
{
	Widget::Widget()
	{
		window = 0;
		document = 0;
	}

	Widget::~Widget()
	{
		Destroy();
	}

	bool Widget::Load(const char* filename)
	{
		if (document)
			Destroy();

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

		return true;
	}

	bool Widget::Create()
	{
		if (!document)
		{
			return 0;
		}

		FrameBuilder fb;
		fb.Register(new Frame());
		fb.Register(new HFrame());
		fb.Register(new VFrame());
		fb.Register(new WindowFrame());
		fb.Register(new Button());
		fb.Build(document);

		window = (WindowFrame*)fb.root;
		if (!window)
			return 0;

		// register mouse events
		window->RegisterEvents(window);

		// stylesheets
		window->stylesheet.LoadStyle(document);
		window->stylesheet.ApplyStyle((Element*)document->OwnerDocument());

		if (0)
		{
			document->Dump();
			window->Dump();
			window->stylesheet.Dump();
			return false;
		} else {
			IWindow *w = (IWindow*)window;
			w->Create(400,200);
			w->Show(true);
		}

		window->Layout();
		return true;
	}

	void Widget::Destroy()
	{
		if (document)
		{
			document->Free();
			document = 0;
		}
	}
}