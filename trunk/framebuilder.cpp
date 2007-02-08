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

#include "framebuilder.h"

namespace Layout
{
	bool FrameBuilder::Register(Frame* f)
	{
		return frameTemplates.Push(f);
	}

	void FrameBuilder::Unregister(Frame* f)
	{
		frameTemplates.Remove(f);
	}

	Frame* FrameBuilder::Build(DOMDocument *doc)
	{
		root = 0;
		CreateFrame(doc);
		return root;
	}

	Frame* FrameBuilder::CreateFrame(Element *element)
	{
		Frame *frame = CreateFrame(element->TagName());
		if (frame)
		{
			if (!root)
				root = frame;

			Frame *parent = frameStack.GetLast();
			if (parent)
			{
				parent->AddFrame(frame);
			}

			frameStack.Push(frame);
		}

		Element *e = (Element*)element->FirstChild();
		while(e)
		{
			CreateFrame(e);
			e = (Element*)e->NextSibling();
		}

		if (frame)
		{
			frameStack.Pop();
		}

		return 0;
	}

	Frame* FrameBuilder::CreateFrame(DOMString *tagName)
	{
		Frame *f = frameTemplates.GetFirst();
		while(f)
		{
			if (strcmp(f->GetName(), tagName->c_str()) == 0)
			{
				return f->Create();
			}
			f = f->next;
		}
		return 0;
	}

}