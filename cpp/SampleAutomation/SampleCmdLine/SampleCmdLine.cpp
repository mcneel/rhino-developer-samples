/////////////////////////////////////////////////////////////////////////////
// SampleCmdLine.cpp
//

#include "stdafx.h"
#include "SampleCmdLine.h"
#include "CRhino5Application.h"
#include "CRhino5Interface.h"
#include "CRhino5x64Application.h"
#include "CRhino5x64Interface.h"
#include "CSampleRhinoObject.h"

// The one and only application object
CWinApp theApp;

using namespace std;

static CString SampleRhinoPlugInID()
{
  // From SampleRhinoPlugIn.cpp
  CString str( L"AD892781-2525-4FC2-85BB-EDF0ADA937AB" );
  return str;
}

static CString SampleRhinoObjectID()
{
  // From SampleRhinoObject.cpp
  CString str( L"8A843D40-7CB2-4567-BC70-F7743FA5BBCE" );
  return str;
}

int wmain( int argc, TCHAR* argv[], TCHAR* envp[] )
{
	HMODULE hModule = ::GetModuleHandle(0);
	if( 0 == hModule )
  {
		wprintf( L"Fatal Error: GetModuleHandle failed\n" );
    return 1;
  }

	if( !AfxWinInit(hModule, 0, ::GetCommandLine(), 0) )
	{
		wprintf( L"Fatal Error: MFC initialization failed\n" );
		return 1;
	}

  CoInitializeEx( 0, COINIT_APARTMENTTHREADED );

#if defined(WIN64)
  //CRhino5x64Application app;
  //BOOL rc = app.CreateDispatch( L"Rhino5x64.Application", 0 );
  CRhino5x64Interface app;
  BOOL rc = app.CreateDispatch( L"Rhino5x64.Interface", 0 );
#else
  //CRhino5Application app;
  //BOOL rc = app.CreateDispatch( L"Rhino5.Application", 0 );
  CRhino5Interface app;
  BOOL rc = app.CreateDispatch( L"Rhino5.Interface", 0 );
#endif
  if( !rc )
  {
    CoUninitialize();
    wprintf( L"Fatal Error: Rhino object creation failed\n" );
		return 1;
  }

  const int bail_milliseconds = 15 * 1000;
  int time_waiting = 0;
  while( 0 == app.IsInitialized() )
  {
    Sleep( 100 );
    time_waiting += 100;
    if( time_waiting > bail_milliseconds )
    {
      CoUninitialize();
      wprintf( L"Fatal Error: Rhino initialization failed\n" );
		  return 1;
    }
  }

  LPUNKNOWN lpUnknown = app.GetPlugInObject( SampleRhinoPlugInID(), SampleRhinoObjectID() );
  if( 0 == lpUnknown )
  {
    app.ReleaseDispatch();
    CoUninitialize();
    wprintf( L"Fatal Error: SampleRhino object creation failed\n" );
    return 1;
  }

  LPDISPATCH lpDispatch = 0;
  HRESULT hr = lpUnknown->QueryInterface( IID_IDispatch, (void**)&lpDispatch );
  if( FAILED(hr) )
  {
    lpUnknown->Release();
    app.ReleaseDispatch();
    CoUninitialize();
    wprintf( L"Fatal Error: SampleRhino object creation failed\n" );
    return 1;
  } 

	CSampleRhinoObject plugin;
  plugin.AttachDispatch( lpDispatch, TRUE );

  CString str = plugin.Hello();
  wprintf( L"%s\n", str );

  plugin.ReleaseDispatch();
  app.ReleaseDispatch();

  CoUninitialize();

	return 0;
}
