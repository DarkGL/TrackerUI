#pragma once

#if defined DEBUG
extern void logToFile( const char * text );
#endif

#include <wininet.h>

#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <memory>
#include <string>

#include <cstdio>

#include <windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>

#include "Server.h"

class ServersLoad
{
    private:
        static const char * checkFavorites;
        static const char * checkGamemenu;
    public:
        void loadFavorites( std::vector< std::unique_ptr< Server > > & servers );
        void loadGamemenu( std::vector< std::unique_ptr< Server > > & servers );
};
