
#ifndef _CHOOKER_H_
#define _CHOOKER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <Psapi.h>
#include <windows.h>
#include <io.h>

#define GET_EAX_POINTER(x) __asm mov x, eax;
#define PAGESIZE 4096

#define GET_ORIG_FUNC(x) CFunc *x; GET_EAX_POINTER(x);

typedef int BOOL;

enum SignatureEntryType
{
	SpecificByte,
	AnyByteOrNothing,
	AnyByte
};

class CMemory
{
	public:

		unsigned char* signature;
		unsigned char* signatureData;

		int sigsize;

		char *baseadd;
		char *endadd;
		char *library;

		CMemory() : signature( 0 ), signatureData( 0 ), sigsize( 0 ), baseadd( ( char* )0xffffffff ), endadd( 0 ), library( 0 ) {}

		BOOL ChangeMemoryProtection( void* function, unsigned int size, unsigned long newProtection )
		{
            FlushInstructionCache( GetCurrentProcess(), function, size );

            static DWORD oldProtection;
            return VirtualProtect( function, size, newProtection, &oldProtection );
		}

		BOOL ChangeMemoryProtection( void* address, unsigned int size, unsigned long newProtection, unsigned long &oldProtection )
		{

            FlushInstructionCache( GetCurrentProcess(), address, size );

            return VirtualProtect( address, size, newProtection, &oldProtection );
		}

		void SetupSignature( char *src )
		{
			int len = strlen( src );

			unsigned char *sig = new unsigned char[ len ];

			signature		= new unsigned char[ len ];
			signatureData	= new unsigned char[ len ];

			unsigned char *s1 = signature;
			unsigned char *s2 = signatureData;

			sigsize = 0;

			memcpy( sig, src, len + 1 );

			char *tok = strtok( ( char* )sig, "," );

			while( tok )
			{
				sigsize++;

				if( strstr( tok, "0x" ) )
				{
					*s1 = strtol( tok, NULL, 0 );
					*s2 = SpecificByte;

					*s1++;
					*s2++;
				}
				else
				{
					*s1 = 0xff;
					*s1++;

					switch( *tok )
					{
					case '*':
						{
							*s2 = AnyByte;
							break;
						}
					case '?':
						{
							*s2 = AnyByteOrNothing;
							break;
						}
					}

					*s2++;
				}

				tok = strtok( NULL, "," );
			}

			delete[] sig;
		}

		BOOL CompareSig( unsigned char *address, unsigned char *signature, unsigned char *signaturedata, int length )
		{
			if( length == 1 )
			{
				switch( *signaturedata )
				{
					case AnyByteOrNothing:
					case AnyByte:
					{
						return true;
					}
					case SpecificByte:
					{
						return ( *address == ( byte )*signature );
					}
				}
			}
			else
			{
				switch( *signaturedata )
				{
					case SpecificByte:
					{
						if( *address != ( byte )*signature )
							return false;
						else
							return CompareSig( address + 1, signature + 1, signaturedata + 1, length - 1 );
					}
					case AnyByteOrNothing:
					{
						if( CompareSig( address, signature + 1, signaturedata + 1, length - 1 ) )
							return true;
					}
					case AnyByte:
					{
						return CompareSig( address + 1, signature + 1, signaturedata + 1, length - 1 );
					}
				}

			}

			return true;
		}

		template <typename T>
		T SearchSignatureByAddress( T* p, char *sig, void* baseaddress, void* endaddress )
		{
			SetupSignature( sig );

			T ret = *p = NULL;

			unsigned char *start = ( unsigned char* )baseaddress;
			unsigned char *end = ( ( unsigned char* )endaddress ) - sigsize;

			unsigned int length = end - start ;

			if( ChangeMemoryProtection( start, length, PAGE_EXECUTE_READWRITE ) )
			{
				for( unsigned int i = 0; i <= length - sigsize; i++ )
				{
					if( CompareSig( start + i, signature, signatureData, sigsize ) )
					{
						*p = ret = (T)( start + i );
						break;
					}
				}
			}

		//	delete[] signature;
	//		delete[] signatureData;

			return ret;
		}

		template <typename T>
		T SearchSymbolByAddress( T* p, char *symbol, void *baseaddress )
		{
			void *handle = ( void* )0xffffffff;

            HMODULE module;

            if( GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, ( LPCSTR )baseaddress, &module ) )
            {
                void* s = GetProcAddress( module, symbol );

                if( s )
                {
                    *p = (T)s;
                    return (T)s;
                }
            }

			return false;
		}

		template <typename T>
		T SearchSignatureByLibrary( T* p, char *sig, char *library )
		{
			return SearchByLibrary( p, sig, library, FALSE );
		}

		template <typename T>
		T SearchSymbolByLibrary( T* p, char *symbol, char *library )
		{
			return SearchByLibrary( p, symbol, library, TRUE );
		}

		template <typename T>
		T SearchByLibrary( T* p, char *data, char *library, BOOL symbol )
		{
			void *baseaddress = baseaddress = GetLibraryFromName( library );

			if( symbol )
				return SearchSymbolByAddress( p, data, baseaddress );
			else
				return SearchSignatureByAddress( p, data, baseadd, endadd );
		}

		template <typename T>
		T SearchByLibrary( T* p, char *data, void* address, BOOL symbol )
		{
			void *baseaddress = address;

			if( symbol )
				return SearchSymbolByAddress( p, data, baseaddress );
			else
				return SearchSignatureByAddress( p, data, baseadd, endadd );
		}

		void* GetLibraryFromAddress( void* addressContained )
		{
				HMODULE module;

				if( GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, ( LPCSTR )addressContained, &module ) )
				{
					HANDLE process =  GetCurrentProcess();
					_MODULEINFO moduleInfo;

					if( GetModuleInformation( process, module, &moduleInfo, sizeof moduleInfo ) )
					{
						CloseHandle( process );

						baseadd = ( char* )moduleInfo.lpBaseOfDll;
						endadd = ( char* )( baseadd + moduleInfo.SizeOfImage );

						return ( void* )baseadd;
					}
				}

				return NULL;
		}

		void* GetLibraryFromName( char *libname )
		{
				HMODULE hMods[ 1024 ];
				HANDLE	hProcess;
				DWORD	cbNeeded;

				unsigned int	i;
				static char		msg[100];

				hProcess = GetCurrentProcess();

				if( hProcess == NULL ) // IS NOT POSSIBLE!
					return NULL;

				if( EnumProcessModules( hProcess, hMods, sizeof( hMods ), &cbNeeded ) )
				{
					TCHAR szModName[ MAX_PATH ];
					_MODULEINFO info;

					for( i = 0; i < ( cbNeeded / sizeof( HMODULE ) ); i++ )
					{
						if( GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof( szModName ) / sizeof( TCHAR ) ) )
						{
							if( strstr( szModName, libname ) > 0 )
							{
								if( GetModuleInformation( hProcess, hMods[i], &info, sizeof( info ) ) )
								{
									baseadd = ( char* )info.lpBaseOfDll;
									endadd	= ( char* )( baseadd + info.SizeOfImage );

									return ( void* )baseadd;
								}
							}
						}
					}
				}

			return NULL;
		}
};

class CFunc
{
	private:

		void* address;
		void* detour;

		CMemory* memFunc;

		unsigned char i_original[12];
		unsigned char i_patched[12];
		unsigned char *original;
		unsigned char *patched;

		bool ispatched;
		bool ishooked;

	public:

		CFunc( void* src, void* dst )
		{
			address		= src;
			detour		= dst;
			ishooked	= ispatched = 0;
			original	= &i_original[0];
			patched		= &i_patched[0];

			memFunc = new CMemory;
		};

		~CFunc()
		{
			delete memFunc;
		};

		void *Hook( void *dst )
		{
			if( !ishooked && !ispatched )
			{
				unsigned int *p;
				detour = dst;

				memcpy( original, address, 12 );

				// lea    this ,%eax
				patched[0] = 0x8D;
				patched[1] = 0x05;

				p = ( unsigned int* )( patched + 2 );
				*p = ( unsigned int )this;

				// nop
				patched[6] = 0x90;

				// jmp detour
				patched[7] = 0xE9;
				p = ( unsigned int* )( patched + 8 );
				*p = ( unsigned int )dst - ( unsigned int )address - 12;

				if( Patch() )
				{
					return address;
				}

				ishooked = false;
			}

			return NULL;
		}

		void *GetOriginal()
		{
			return address;
		}

		bool Patch()
		{
			if( !ispatched )
			{
				if( memFunc->ChangeMemoryProtection( address, PAGESIZE, PAGE_EXECUTE_READWRITE ) )
				{
					memcpy( address, patched, 12 );
					ispatched = true;
				}
			}

			return ispatched;
		}

		bool Restore()
		{
			if( ispatched )
			{
				if( memFunc->ChangeMemoryProtection( address, PAGESIZE, PAGE_EXECUTE_READWRITE ) )
				{
					memcpy( address, original, 12 );
					ispatched = false;
				}
			}

			return !ispatched;
		}
};

class CHooker
{
	private:

		struct Obj
		{
			void*	src;
			CFunc*	func;
			Obj*	next;
		} *head;

	public:

		CMemory* memFunc;

		CHooker() : head( 0 )
		{
			memFunc = new CMemory;
		};

		~CHooker()
		{
			Clear();
		};

		void Clear()
		{
			while( head )
			{
				Obj *obj = head->next;

				delete head->func;
				delete head;

				head = obj;
			}

			delete memFunc;
		}

		template <typename Tsrc, typename Tdst>
		CFunc* AddHook(Tsrc src, Tdst dst)
		{
			if( !src || !dst )
				return NULL;

			Obj *obj = head;

			if( !obj )
			{
				head = new Obj();
				obj = head;

				obj->src = ( void* )src;
				obj->func = new CFunc( ( void* )src, ( void* )dst);
				obj->next = NULL;
			}
			else
			{
				while( obj )
				{
					if( obj->src == ( void* )src )
					{
						break;
					}
					else if( !obj->next )
					{
						obj->next = new Obj();
						obj = obj->next;

						obj->src = ( void* )src;
						obj->func = new CFunc( ( void* )src, ( void* )dst );
						obj->next = NULL;

						break;
					}
					obj = obj->next;
				}
			}

			obj->func->Hook( ( void* )dst );

			return obj->func;
		}
};

#endif // _CHOOKER_H_

