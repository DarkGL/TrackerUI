#include "ServersLoad.h"

const char * ServersLoad::checkFavorites = "http://cssetti.pl/pliki/stats/favorites.php";
const char * ServersLoad::checkGamemenu = "http://cssetti.pl/pliki/stats/gamemenu.php";

void ServersLoad::loadFavorites( std::vector< std::unique_ptr< Server > > & servers ){
    HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	if( !connectHandle ){
    	return;
	}

	HINTERNET openAddress = InternetOpenUrl( connectHandle , ServersLoad::checkGamemenu , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

	if ( !openAddress ){
    	InternetCloseHandle( connectHandle );
    	return;
	}

	DWORD statCodeLen = sizeof(DWORD);
    DWORD statCode;

	if(!HttpQueryInfo(openAddress,
                      HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                      &statCode,
                      &statCodeLen,
                      NULL))
    {
    	InternetCloseHandle( openAddress );
        InternetCloseHandle( connectHandle );
    	return;
    }

    if( statCode != HTTP_STATUS_OK ){
    	InternetCloseHandle( openAddress );
    	InternetCloseHandle( connectHandle );
    	return;
    }

    std::string fullBuffer;

    char bufferRead[ 1024 ];
	DWORD NumberOfBytesRead = 0;

	while( InternetReadFile( openAddress , bufferRead , ( sizeof bufferRead ) - 1, &NumberOfBytesRead ) && NumberOfBytesRead ){

        bufferRead[ NumberOfBytesRead ] = '\0';

		fullBuffer.append( bufferRead );
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );

  	#if defined DEBUG
	char bufferLog[ 512 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Loaded favorites servers %s" , fullBuffer.c_str() );
	logToFile( bufferLog );
	#endif

    std::string delimiter = "\n";

	size_t pos = 0;
    std::string token;

    int currentState = 0;

    std::unique_ptr<Server> currentServer;

    while ((pos = fullBuffer.find(delimiter)) != std::string::npos) {
        token = fullBuffer.substr(0, pos);

        fullBuffer.erase(0, pos + delimiter.length());

        #if defined DEBUG
        char bufferLog[ 256 ];
        snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Loaded favorites server %s" , token.c_str() );
        logToFile( bufferLog );
        #endif

        if( currentState % 2 == 0 ){
            currentState = 1;

            currentServer = std::move( std::unique_ptr<Server> (new Server) );

            currentServer -> setName( token );
        }
        else{
             currentState = 0;

             currentServer -> setIp( token );

             servers.push_back( std::move( currentServer ) );
        }
    }
}

void ServersLoad::loadGamemenu( std::vector< std::unique_ptr< Server > > & servers ){
   HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	if( !connectHandle ){
    	return;
	}

	HINTERNET openAddress = InternetOpenUrl( connectHandle , ServersLoad::checkGamemenu , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

	if ( !openAddress ){
    	InternetCloseHandle( connectHandle );
    	return;
	}

	DWORD statCodeLen = sizeof(DWORD);
    DWORD statCode;

	if(!HttpQueryInfo(openAddress,
                      HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                      &statCode,
                      &statCodeLen,
                      NULL))
    {
    	InternetCloseHandle( openAddress );
        InternetCloseHandle( connectHandle );
    	return;
    }

    if( statCode != HTTP_STATUS_OK ){
    	InternetCloseHandle( openAddress );
    	InternetCloseHandle( connectHandle );
    	return;
    }

    std::string fullBuffer;

    char bufferRead[ 1024 ];
	DWORD NumberOfBytesRead = 0;

	while( InternetReadFile( openAddress , bufferRead , ( sizeof bufferRead ) - 1, &NumberOfBytesRead ) && NumberOfBytesRead ){

        bufferRead[ NumberOfBytesRead ] = '\0';

		fullBuffer.append( bufferRead );
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );

  	#if defined DEBUG
	char bufferLog[ 512 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Loaded gamemenu servers %s" , fullBuffer.c_str() );
	logToFile( bufferLog );
	#endif

    std::string delimiter = "\n";

	size_t pos = 0;
    std::string token;

    int currentState = 0;

    std::unique_ptr<Server> currentServer;

    while ((pos = fullBuffer.find(delimiter)) != std::string::npos) {
        token = fullBuffer.substr(0, pos);

        fullBuffer.erase(0, pos + delimiter.length());

        #if defined DEBUG
        char bufferLog[ 256 ];
        snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Loaded gamemenu server %s" , token.c_str() );
        logToFile( bufferLog );
        #endif

        if( currentState % 2 == 0 ){
            currentState = 1;

            currentServer = std::move( std::unique_ptr<Server> (new Server) );

            currentServer -> setName( token );
        }
        else{
             currentState = 0;

             currentServer -> setIp( token );

             servers.push_back( std::move( currentServer ) );
        }
    }
}
