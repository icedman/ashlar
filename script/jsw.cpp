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

Credits:
Franky Braem - wxJS
*/

#include <script/jsw.h>
#include <string.h>

JSBool JSQueryObject::IsType(const char* name)
{
	return strcmp(JSQueryObject::GetClassName(), name);
}

JSW_CLASS_NAME(JSQueryObject, "_tmpQuery")

JSW_BEGIN_PROPERTY_MAP(JSQueryObject)
JSW_END_PROPERTY_MAP

JSW_BEGIN_METHOD_MAP(JSQueryObject)
JSW_END_METHOD_MAP