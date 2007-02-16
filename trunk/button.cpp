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

#include "button.h"

namespace Layout
{
	bool Button::OnEvent(int eid, void *pInfo)
	{
		MouseInfo *mInfo = (MouseInfo*)pInfo;

		Rect r;
		GetRect(&r);

		if (!r.Contains(mInfo->point))
		{
			if (GetState() == PRESSED || GetState() == HOVER)
				SetState(NORMAL);
			return true;
		}

		switch(eid)
		{
		case ONMOUSEMOVE:
			if (GetState() != PRESSED && GetState() != HOVER)
				SetState(HOVER);
			break;
		case ONMOUSEDOWN:
			SetState(PRESSED);
			break;
		case ONMOUSEUP:
			if (GetState() == PRESSED)
			{
				printf("OnClick!\n");
			}
			SetState(HOVER);
			break;
		}

		return true;
	}

	bool Button::RegisterEvents(EventManager *manager)
	{
		manager->AddListener(new Event(ONMOUSEDOWN, this));
		manager->AddListener(new Event(ONMOUSEUP, this));
		manager->AddListener(new Event(ONMOUSEMOVE, this));
		return true;
	}
}