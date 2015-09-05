#pragma once

#include <exdispid.h>

extern IWebBrowser2* browsers[ 256 ];
extern int currentBrowser;

class BrowserEvents: public DWebBrowserEvents2
{
	public:
		STDMETHODIMP Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr){
			switch(dispIdMember){
    			case DISPID_NEWWINDOW2:{
    				*( pDispParams->rgvarg[1].pboolVal ) = VARIANT_FALSE;

   					HRESULT hr = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER,
                       IID_IWebBrowser2, (void**)&browsers[ currentBrowser ]);

                    browsers[ currentBrowser ]->put_Visible( VARIANT_FALSE );
           			browsers[ currentBrowser ]->put_FullScreen( VARIANT_TRUE );

    				*(pDispParams->rgvarg[0].ppdispVal) = (IDispatch*)browsers[ currentBrowser ];

					currentBrowser++;

    				break;
    			}
			}

			return S_OK;
		}

		STDMETHODIMP QueryInterface(REFIID riid, void ** ppvObj){
			(*ppvObj)=NULL;

			if(!IsEqualIID(riid,IID_IUnknown) && !IsEqualIID(riid,IID_IDispatch) && !IsEqualIID(riid,DIID_DWebBrowserEvents2)) return E_NOINTERFACE;

			(*ppvObj)=(void*)this;
			return S_OK;
		}

		STDMETHODIMP_(ULONG) AddRef(){
				return 1;
		}
		STDMETHODIMP_(ULONG) Release(){
				return 1;
		}

		STDMETHODIMP GetTypeInfoCount(UINT *iTInfo){
				return E_NOTIMPL;
		}
		STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo){
				return E_NOTIMPL;
		}
		STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR **rgszNames,
                              UINT cNames,  LCID lcid, DISPID *rgDispId){
				return E_NOTIMPL;
		}
};
