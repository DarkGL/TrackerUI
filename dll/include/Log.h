#pragma once

#if defined DEBUG
void logToFile( const char * text ){
	FILE * fp = fopen( "log.txt" , "at" );

	if( !fp ){
		return;
	}

    std::time_t result = std::time(NULL);

	char bufferLog[ 2048 ];

	snprintf( bufferLog , sizeof( bufferLog ) - 1 , "[%s] %s\n\r", asctime( std::localtime( &result ) ) , text );

	fputs( bufferLog , fp );

	fclose( fp );
}

char* asctime(const struct tm *timeptr){
  static char result[26];
  sprintf(result, "%.2d:%.2d:%.2d",
    timeptr->tm_hour,
    timeptr->tm_min, timeptr->tm_sec);
  return result;
}
#endif
