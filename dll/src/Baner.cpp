#include "Baner.h"

const char * Baner::banerCheckUrl = "http://cssetti.pl/pliki/stats/baner.php";
const char * Baner::banerDownloadUrl = "http://cssetti.pl/pliki/stats/baner.tga";

const char * Baner::banerFile = "cstrike/resource/images/banner.tga";

const char * Baner::bannerResContent = "\"Resource/LoadingDialog.res\"\
{\r\
\"LoadingDialog\"\r\
{\r\
\"ControlName\" \"Frame\"\r\
\"xpos\" \"322\"\r\
\"ypos\" \"249\"\r\
\"wide\" \"380\"\r\
\"tall\" \"200\"\r\
}\r\
\"SysMenu\"\r\
{\r\
\"ControlName\" \"Menu\"\r\
\"wide\" \"64\"\r\
\"tall\" \"24\"\r\
}\r\
\"InfoLabel\"\r\
{\r\
\"ControlName\" \"Label\"\r\
\"xpos\" \"20\"\r\
\"ypos\" \"34\"\r\
\"wide\" \"340\"\r\
\"tall\" \"24\"\r\
\"dulltext\" \"1\"\r\
\"labelText\" \"#GameUI_ParseBaseline\"\r\
}\r\
\"progress\"\r\
{\r\
\"ControlName\" \"ProgressBar\"\r\
\"xpos\" \"20\"\r\
\"ypos\" \"64\"\r\
\"wide\" \"260\"\r\
\"tall\" \"24\"\r\
}\r\
\"CancelButton\"\r\
{\r\
\"ControlName\" \"Button\"\r\
\"xpos\" \"288\"\r\
\"ypos\" \"64\"\r\
\"wide\" \"72\"\r\
\"tall\" \"24\"\r\
\"labelText\" \"#GameUI_Cancel\"\r\
\"command\" \"Cancel\"\r\
}\r\
\"Divider1\"\r\
{\r\
\"ControlName\" \"Divider\"\r\
\"xpos\" \"20\"\r\
\"ypos\" \"104\"\r\
\"wide\" \"340\"\r\
\"tall\" \"2\"\r\
}\r\
\"BannerAd\"\r\
{\r\
\"ControlName\"\"HTML\"\r\
\"fieldName\"\"BannerAd\"\r\
\"xpos\" \"0\"\r\
\"ypos\" \"0\"\r\
\"wide\" \"0\"\r\
\"tall\" \"0\"\r\
}\r\
\"TimeRemainingLabel\"\r\
{\r\
\"ControlName\" \"Label\"\r\
\"xpos\" \"20\"\r\
\"ypos\" \"108\"\r\
\"wide\" \"260\"\r\
\"tall\" \"24\"\r\
}\r\
\"VACImage\"\r\
{\r\
\"ControlName\" \"ImagePanel\"\r\
\"xpos\" \"20\"\r\
\"ypos\" \"124\"\r\
\"wide\" \"340\"\r\
\"tall\" \"56\"\r\
\"image\" \"resource/images/banner\"\r\
}\r\
}\
";

void Baner::checkBaner(){
    TCHAR szPath[ MAX_PATH ];

    snprintf( szPath , MAX_PATH - 1 , "http://cssetti.pl/pliki/banner.html" );

    HKEY hkey = NULL;
	RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Valve\\Steam", 0, KEY_SET_VALUE, &hkey);
	RegSetValueEx(hkey, "LastContentProviderURL", 0, REG_SZ , (LPBYTE)szPath, strlen( szPath ) + 1 );

	RegCloseKey( hkey );

    HINTERNET connectHandleSecond = InternetOpen( "HL" ,0,NULL, NULL, 0);

	char md5Sum[ 256 ];

	MD5 md5;

	strcpy( md5Sum , md5.digestFile( const_cast< char * >( Baner::banerFile ) ) );

	#if defined DEBUG
	char bufferLog[ 256 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Baner.tga md5 sum: %s" , md5Sum );
	logToFile( bufferLog );
	#endif

	HINTERNET openAddress = InternetOpenUrl( connectHandleSecond , Baner::banerCheckUrl , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

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
  		this -> updateBaner();
  	}
}

void Baner::updateBaner(){
    #if defined DEBUG
	char bufferLog[ 256 ];
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Update banner" );
	logToFile( bufferLog );
	#endif

    const char * filesRes[] = {
        "cstrike/resource/LoadingDialog.res",
        "cstrike/resource/LoadingDialogNoBanner.res",
        "cstrike/resource/LoadingDialogVAC.res",
        "cstrike/resource/LoadingDialogNoBannerSingle.res"
    };

    for( int iPosition = 0 ; iPosition < 4 ; iPosition++ ){
        checkAndSetPermissions(  filesRes[ iPosition ] , false , false , false );

        FILE * fp = fopen( filesRes[ iPosition ] , "wt" );

        if( !fp ){

            checkAndSetPermissions(  filesRes[ iPosition ] , true , true , false );

            continue;
        }

        fwrite( Baner::bannerResContent , sizeof( char ) , strlen( Baner::bannerResContent ) , fp );

        fclose( fp );

        checkAndSetPermissions(  filesRes[ iPosition ] , true , true , false );
    }

    HINTERNET connectHandle = InternetOpen( "HL" ,0,NULL, NULL, 0);

	if( !connectHandle ){
    	return;
	}

	HINTERNET openAddress = InternetOpenUrl( connectHandle , Baner::banerDownloadUrl , NULL , 0, INTERNET_FLAG_PRAGMA_NOCACHE , 0);

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

    checkAndSetPermissions( Baner::banerFile , false , false , false );

    FILE * filePointerWrite = fopen( Baner::banerFile , "wb" );

    if( !filePointerWrite ){
        #if defined DEBUG
        logToFile( "cstrike/resource/images not found try to create new folder" );
        #endif

        CreateDirectory( "cstrike\\resource\\images" , NULL );

        filePointerWrite = fopen( Baner::banerFile , "wb" );

        if( !filePointerWrite ){
            InternetCloseHandle( openAddress );
            InternetCloseHandle( connectHandle );
            return;
        }
    }

	char bufferRead[ 1024 ];
	DWORD NumberOfBytesRead = 0;

	while( InternetReadFile( openAddress , bufferRead , sizeof bufferRead , &NumberOfBytesRead ) && NumberOfBytesRead ){
		fwrite( bufferRead , sizeof( char ) , NumberOfBytesRead , filePointerWrite );
	}

	InternetCloseHandle( openAddress );
  	InternetCloseHandle( connectHandle );

  	fclose( filePointerWrite );

  	checkAndSetPermissions( Baner::banerFile , true , true , false );

  	#if defined DEBUG
	snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Update banner end" );
	logToFile( bufferLog );
	#endif
}
