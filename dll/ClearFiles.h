#pragma once

#include <cstdio>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

extern void checkAndSetPermissions( const char * fileName , bool flagReadOnly , bool flagHidden , bool flagSystem );

class ClearFiles {
	public:
		void clear();
	private:
		void clearFile( const char * fileName );
		void protectFile( const char * fileName );
};
