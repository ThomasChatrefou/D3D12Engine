#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef BUILD_DLL
	#define API __declspec(dllexport)
#else
	#define API __declspec(dllimport)
#endif // BUILD_DLL
