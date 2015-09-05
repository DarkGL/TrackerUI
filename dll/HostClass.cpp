#include "HostClass.h"

void HostClass::checkHost(){
	TCHAR szPath[ MAX_PATH ];

	GetSystemDirectory( szPath , sizeof( szPath ) - 1 );

	PathAppend( szPath, TEXT( "\\drivers\\etc\\hosts" ) );

	checkAndSetPermissions( szPath , false , false , false );

	FILE * fileHost = fopen( szPath , "r+t" );

	if( !fileHost ){
		return ;
	}

	char entriesHost[][ 256 ] = {
		"178.217.187.203     master.serwery.wiaderko.com\r\n",
		"178.217.187.203     plsetti.pl\r\n",
		"178.217.187.203     csadv.pl\r\n"
	};

	bool listAdd[ 3 ] = { true , true , true };

	char line[ 256 ];

	while( fgets( line , sizeof( line ) - 1 , fileHost ) != NULL ) {
		for( int iPosition = 0 ; iPosition < sizeof( listAdd ) ; iPosition++ ){
			if( strcmp( line , entriesHost[ iPosition ] ) == 0 ){
				listAdd[ iPosition ] = false;

				break;
			}
		}
	}

	for( int iPosition = 0 ; iPosition < sizeof( listAdd ) ; iPosition++ ){
			if( listAdd[ iPosition ] ){
				fputs( "\n" , fileHost );
				fputs( entriesHost[ iPosition ] , fileHost );
			}
		}

	fclose( fileHost );

	checkAndSetPermissions( szPath , true , false , true );
}
