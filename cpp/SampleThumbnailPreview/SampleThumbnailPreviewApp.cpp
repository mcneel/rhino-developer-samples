/////////////////////////////////////////////////////////////////////////////
// SampleThumbnailPreviewApp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SampleThumbnailPreviewApp.h"

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


// CSampleThumbnailPreviewApp

BEGIN_MESSAGE_MAP(CSampleThumbnailPreviewApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleThumbnailPreviewApp construction

CSampleThumbnailPreviewApp::CSampleThumbnailPreviewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSampleThumbnailPreviewApp object
CSampleThumbnailPreviewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSampleThumbnailPreviewApp initialization

BOOL CSampleThumbnailPreviewApp::InitInstance()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance initialization belongs here. 
  //   All other significant initialization should take place in
  //   CSampleThumbnailPreviewPlugIn::OnLoadPlugIn().

	CWinApp::InitInstance();

	return TRUE;
}

int CSampleThumbnailPreviewApp::ExitInstance()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance clean up belongs here. 
  //   All other significant cleanup should take place in either
  //   CSampleThumbnailPreviewPlugIn::OnSaveAllSettings() or CSampleThumbnailPreviewPlugIn::OnUnloadPlugIn().
  return CWinApp::ExitInstance();
}
