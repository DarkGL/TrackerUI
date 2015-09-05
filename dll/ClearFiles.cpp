#include "ClearFiles.h"

void ClearFiles::clear(){
	char fileNames[][ 256 ] = {
		"cstrike/autoexec.cfg",
		"cstrike/language.cfg",
		"cstrike/joystick.cfg",
		"cstrike/violence.cfg",
		"cstrike/default.cfg"
		"cstrike/(null).cfg",
		"cstrike/autoconfig.cfg",
		"cstrike/banner.cfg",
		"cstrike/listenserver.cfg"
		"cstrike/game.cfg"
	};

	char filesProtect[][ 256 ] = {
		"cstrike/userconfig.cfg",
		"cstrike/server.cfg",
		"cstrike/valve.rc",
		"valve/valve.rc"
		"cstrike/config.cfg"
		"valve/client_setting.ini",
		"cstrike/commandmenu.txt"
		"voice_mp3.asi",
		"voice_mp3.dll",
		"rev.ini",
		"cstrike/resource/GameMenu.res",
		"cstrike/resource/LoadingDialog.res",
		"cstrike/resource/LoadingDialogNoBanner.res",
		"cstrike/resource/LoadingDialogVAC.res",
		"valve/resource/LoadingDialog.res",
		"valve/resource/LoadingDialogNoBanner.res",
		"valve/resource/LoadingDialogVAC.res",
		"valve/hw/opengl.cfg",
		"valve/hw/geforce.cfg",
		"cstrike_russian/hw/opengl.cfg",
        "cstrike_russian/resource/gamemenu.res",
        "cstrike/resource/UI/MOTD.RES"
	};

	for( int iPosition = 0; iPosition < sizeof( fileNames ) / sizeof( fileNames[ 0 ] ) ; iPosition++ ){
		this -> clearFile( fileNames[ iPosition ] );
	}

	for( int iPosition = 0; iPosition < sizeof( filesProtect ) / sizeof( filesProtect[ 0 ] ) ; iPosition++ ){
		this -> protectFile( filesProtect[ iPosition ] );
	}
}

void ClearFiles::clearFile( const char * fileName ){
	checkAndSetPermissions( fileName , false , false , false );

	char oldName[ 256 ];

  	snprintf( oldName , sizeof( oldName ) , "%s.old" , fileName );

  	MoveFile( fileName , oldName );

	FILE * filePointer = fopen( fileName , "wt" );

	fclose( filePointer );

	checkAndSetPermissions( fileName , true , true , false );
}

void ClearFiles::protectFile( const char * fileName ){
	checkAndSetPermissions( fileName , true , false , false );
}
