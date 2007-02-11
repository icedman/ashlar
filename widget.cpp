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

		DOMBuilder dbuild;
		dbuild.Initialize();

		FILE *fp = fopen(filename, "r");
		while(!feof(fp))
		{
			char buffer[1024];
			unsigned short len = fread(buffer, 1, 1024, fp);
			int isFinal = feof(fp);
			dbuild.Parse(buffer, len, isFinal);
		}
		fclose(fp);

		document = dbuild.GetDocument();
		dbuild.Shutdown();

		return Create();
	}

	bool Widget::Create()
	{
		if (!document)
		{
			return 0;
		}

		document->Dump();

		FrameBuilder fbuild;
		fbuild.Register(new Frame());
		fbuild.Register(new HFrame());
		fbuild.Register(new VFrame());
		fbuild.Register(new WindowFrame());
		fbuild.Register(new Button());
		fbuild.Build(document);

		window = (WindowFrame*)fbuild.root;
		if (!window)
			return 0;

		window->Dump();

		IWindow *w = (IWindow*)window;
		w->Create(400,200);
		w->Show(true);

		// register mouse events
		window->RegisterEvents(window);
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