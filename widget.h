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

#include "windowframe.h"
#include "button.h"

#include "document.h"
#include "stylesheet.h"
#include "docbuilder.h"
#include "framebuilder.h"

using namespace Dom;
using namespace Layout;
using namespace Events;

namespace Ash
{
	//! Widget class
	class Widget : public Frame
	{
	public:

		Widget();
		virtual ~Widget();

		FRAMETYPE(WIDGET, Frame)

		//! Loads widget from an xul file
		bool Load(const char* filename);
		virtual void Free();

		StyleSheet* GetStyleSheet() { return styleSheet; }
		DOMDocument* GetDocument() { return (DOMDocument*)element; }
		// ScriptEngine* GetScriptEngine()
		// Resources* GetResources()

	private:

		StyleSheet  *styleSheet;
		// JavaScript *scriptEngine;
	};

}