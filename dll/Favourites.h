#pragma once

#include <vector>
#include <memory>

#include "Server.h"

class Favourites{
	private:
		static const char * favouritesFile;
	public:
		void loadServers( std::vector< std::unique_ptr< Server > > & serversOld );
		void saveServers( std::vector< std::unique_ptr< Server > > & servers , std::vector< std::unique_ptr< Server > > & serversOld );
};
