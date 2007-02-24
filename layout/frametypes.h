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

#pragma once

namespace Layout
{
	const unsigned int FRAME	=    0;
	const unsigned int HBOX		=    1;
	const unsigned int VBOX		=    2;
	const unsigned int BOX		=    3;
	const unsigned int LABEL	=  100;
	const unsigned int BUTTON	=  101;
	const unsigned int IMAGE	=  102;
	const unsigned int GRID 	=  200;
	const unsigned int ROW 		=  201;	
	const unsigned int WINDOW	= 1000;
	const unsigned int WIDGET	= 2000;

#define FRAMETYPE(_type, _super) \
	virtual bool IsType(int frameType) { if (frameType == _type) return true; return _super::IsType(frameType); }
}