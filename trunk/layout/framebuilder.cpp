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

#include <layout/framebuilder.h>
#include <layout/framestyle.h>
#include <layout/frametypes.h>

namespace Layout
{
	FrameBuilder::FrameBuilder()
	{
	}

	FrameBuilder::~FrameBuilder()
	{
		Free();
	}

	void FrameBuilder::Free()
	{
		Frame *f;
		while(f = frameTemplates.GetFirst())
		{
			frameTemplates.Remove(f);
			delete f;
		}
	}

	bool FrameBuilder::Register(Frame* f)
	{
		return frameTemplates.Push(f);
	}

	void FrameBuilder::Unregister(Frame* f)
	{
		frameTemplates.Remove(f);
	}

	Frame* FrameBuilder::Build(Document *doc)
	{
		root = 0;
		BuildFrames(doc);
		//root->Dump();
		return root;
	}

	Frame* FrameBuilder::BuildFrames(Element *element)
	{
		Frame *frame = CreateFrame(element);
		if (frame)
		{
			if (!root && frame->IsType(WINDOW))
			{
				root = frame;
				printf("%s\n", frame->GetName());
			}

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
			BuildFrames(e);
			e = (Element*)e->NextSibling();
		}

		if (frame)
		{
			frameStack.Pop();
		}

		return 0;
	}

	Frame* FrameBuilder::CreateFrame(Element *e)
	{
		Frame *f = frameTemplates.GetFirst();
		while(f)
		{
			if (strcmp(f->GetName(), e->TagName()->c_str()) == 0)
			{
				Frame *nw = f->Create();
				e->SetData((void*)nw);
				nw->SetElement(e);
				return nw;
			}
			f = f->next;
		}
		return 0;
	}

}