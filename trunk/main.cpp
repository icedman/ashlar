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

#include <common.h>
#include <widget.h>
#include <render/resources.h>
#include <dom/safenode.h>
#include <win32/nativewindow.h>

using namespace Ash;

int main()
{
	Widget *widget = new Widget();
	if (!widget->Load("ashlar.xul"))
		PostQuitMessage(0);

	printf("unfreed objects:%d\n", Ref::GetCount());

	MSG msg;
	while( GetMessage(&msg, NULL, 0, 0) ) {

		if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
			if( msg.message == WM_QUIT ) break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete widget;

	printf("unfreed objects:%d\n", Ref::GetCount());
	return 0;
}