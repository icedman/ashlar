#pragma once

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#ifdef _DEBUG
#include <crtdbg.h>
#define LEAK_TRACE { _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF); }
#else
#define LEAK_TRACE
#endif