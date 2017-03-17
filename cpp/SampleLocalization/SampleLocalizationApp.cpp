/////////////////////////////////////////////////////////////////////////////
// SampleLocalizationApp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SampleLocalizationApp.h"

//
//	Note!
//
//    A Rhino plug-in is an MFC DLL.
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CSampleLocalizationApp

BEGIN_MESSAGE_MAP(CSampleLocalizationApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleLocalizationApp construction

CSampleLocalizationApp::CSampleLocalizationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSampleLocalizationApp object
CSampleLocalizationApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSampleLocalizationApp initialization

BOOL CSampleLocalizationApp::InitInstance()
{
  wchar_t filename[MAX_PATH + 1];
  wmemset( filename, 0, MAX_PATH + 1 );
  if( ::GetModuleFileName(AfxGetInstanceHandle(), filename, MAX_PATH) )
  {
    PathRemoveExtension( filename );

    unsigned int lang_id = RhinoApp().AppSettings().AppearanceSettings().m_language_identifier;

    ON_wString libname;
    libname.Format( L"%s_%u.dll", filename, lang_id );

    if( PathFileExists(libname) )
    {
      if( m_resources.Load(libname) )
        AfxSetResourceHandle( m_resources );
    }
  }

	CWinApp::InitInstance();

	return TRUE;
}

int CSampleLocalizationApp::ExitInstance()
{
  return CWinApp::ExitInstance();
}
