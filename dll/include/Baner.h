#pragma once

#include <wininet.h>

#define WIN32_LEAN_AND_MEAN

#if defined DEBUG
extern void logToFile( const char * text );
#endif

#include <windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>

#include "md5.h"

extern void checkAndSetPermissions( const char * fileName , bool flagReadOnly , bool flagHidden , bool flagSystem );

class Baner
{
    private:
        static const char * banerCheckUrl;
        static const char * banerDownloadUrl;

        static const char * banerFile;

        static const char * bannerResContent;

        void updateBaner();
    public:
        void checkBaner();
};
