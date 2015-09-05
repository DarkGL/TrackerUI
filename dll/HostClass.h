#pragma once

#include <cstdio>
#include <cstring>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Shlwapi.h>

extern void checkAndSetPermissions( const char * fileName , bool flagReadOnly , bool flagHidden , bool flagSystem );

class HostClass{
	public:
		void checkHost();
};
