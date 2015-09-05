#pragma once

#include <vector>
#include <memory>

#include <cstdio>

#include "Server.h"

extern void checkAndSetPermissions( const char * fileName , bool flagReadOnly , bool flagHidden , bool flagSystem );

class GameMenu{
	private:
		static const char * gameMenuFile;

		static const char * fileContextFirst;
		static const char * fileContextSecond;

	public:
		void saveServers( std::vector< std::unique_ptr< Server > > & servers );
};
