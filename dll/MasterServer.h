#pragma once

#include <cstdio>
#include <cstring>

extern void checkAndSetPermissions( const char * fileName , bool flagReadOnly , bool flagHidden , bool flagSystem );

class MasterServer {
	private:
		static const char * configMaster;
		static const char * configRevMaster;
		
		static const char * platformMaster;
		static const char * platformConfigMaster;
		static const char * platformConfigMaster2;
		static const char * platformConfigRev;
			
		void replaceConfigMaster();
		void replaceRevMaster();
		void replacePlatformConfig();
		void replacePlatformConfigMaster();
		void replacePlatformConfigMaster2();
		void replacePlatformConfigRev();
		
		void _replaceFileContext( const char * fileName , const char * context );
		
	public:
		void replaceMaster();
};
