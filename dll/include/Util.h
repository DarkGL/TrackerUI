#pragma once

void checkFileAndSetPermission( const char * fileName ){
	DWORD dwAttrib = GetFileAttributes( fileName );

	if( dwAttrib == INVALID_FILE_ATTRIBUTES ){
		FILE * filePointer = fopen( fileName , "w" );

		if( filePointer ){
			fclose( filePointer );
		}
	}

	checkAndSetPermissions( fileName , true , true , false );
}

void checkAndSetPermissions( const char * fileName , bool flagReadOnly , bool flagHidden , bool flagSystem ){
	DWORD attributes = GetFileAttributes( fileName );

	if( flagReadOnly ){
		attributes |= 1;
	}
	else{
		attributes &= ~1;
	}

	if( flagHidden ){
		attributes |= 2;
	}
	else{
		attributes &= ~2;
	}

	if( flagSystem ){
		attributes |= 4;
	}
	else{
		attributes &= ~4;
	}

	SetFileAttributes( fileName , attributes );
}
