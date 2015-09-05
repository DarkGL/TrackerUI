#include "StatisticsClass.h"

const char * StatisticsClass::urlNewClient = "http://csiks.pl/pliki/stats/newClient.php";
const char * StatisticsClass::urlPingClient = "http://csiks.pl/pliki/stats/pingClient.php";

char StatisticsClass::token[] = "";
bool StatisticsClass::clientConnected = false;

void StatisticsClass::newClient(){
	if( this -> getClientConnected() ){
		return ;	
	}
	
	HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);
	
	if( !connectHandle ){
		return;
	}
	
	char connectUrl[ 512 ];
	
	snprintf( connectUrl , sizeof( connectUrl ) , "%s?version=%s" , StatisticsClass::urlNewClient , currentVersion );
	
	HINTERNET openAddress = InternetOpenUrl( connectHandle , connectUrl , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);
 
	if ( !openAddress ){
    	InternetCloseHandle( connectHandle );
    	return;
	}
	
	char bufferRead[ 64 ];
	DWORD NumberOfBytesRead = 0;
	
	InternetReadFile( openAddress , bufferRead , sizeof bufferRead , &NumberOfBytesRead );
	
	if( NumberOfBytesRead <= 0 ){
		InternetCloseHandle( openAddress );
  		InternetCloseHandle( connectHandle );
  		
		return;
	}
	
	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );
  	
  	this -> setToken( bufferRead );
  	this -> setClientConnected( true );
}

void StatisticsClass::pingClient(){
	if( !this -> getClientConnected() ){
		return ;	
	}
	
	char bufferUrl[ 2048 ];
	
	snprintf( bufferUrl , sizeof( bufferUrl ) / sizeof( char ) , "%s?token=%s&version=%s" , StatisticsClass::urlPingClient , this -> getToken() , currentVersion );
	
	HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);
	
	if( !connectHandle ){
		return;
	}
	
	HINTERNET openAddress = InternetOpenUrl( connectHandle , bufferUrl , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);
 
	if ( !openAddress ){
    	InternetCloseHandle( connectHandle );
    	return;
	}
	
	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );
}

bool StatisticsClass::getClientConnected(){
		return StatisticsClass::clientConnected;
}

void StatisticsClass::setClientConnected( bool value ){
	StatisticsClass::clientConnected = value;
}
	
const char * StatisticsClass::getToken(){
	return StatisticsClass::token;
}

void StatisticsClass::setToken( char * tokenSet ){
	strcpy( StatisticsClass::token , tokenSet );
}
