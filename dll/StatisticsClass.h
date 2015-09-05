#pragma once

#include <wininet.h>
#include <iphlpapi.h>
#include <cstdio>

extern const char * currentVersion;

class StatisticsClass {
	private:
		static const char * urlNewClient;
		static const char * urlPingClient;
		
		static char token[ 32 ];
		static bool clientConnected;
	public:
		void newClient();
		void pingClient();
		
		bool getClientConnected();
		void setClientConnected( bool value );
		
		const char * getToken();
		void setToken( char * tokenSet ); 
};
