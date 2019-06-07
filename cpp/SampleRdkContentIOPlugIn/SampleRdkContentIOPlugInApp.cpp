
/** This sample demonstrates how to load a Basic Material (AKA Custom Material) from a simple file format.

*/

#include "stdafx.h"
#include "SampleRdkContentIOPlugInApp.h"

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

// CSampleRdkContentIOPlugInRhinoPlugInApp

BEGIN_MESSAGE_MAP(CSampleRdkContentIOPlugInRhinoPlugInApp, CWinApp)
END_MESSAGE_MAP()

// CSampleRdkContentIOPlugInRhinoPlugInApp construction

CSampleRdkContentIOPlugInRhinoPlugInApp::CSampleRdkContentIOPlugInRhinoPlugInApp()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance construction belongs here. 
  //   All other significant initialization should take place in
  //   CSampleRdkContentIOPlugInRhinoPlugIn::OnLoadPlugIn().
}

// The one and only CSampleRdkContentIOPlugInRhinoPlugInApp object
CSampleRdkContentIOPlugInRhinoPlugInApp theApp;

// CSampleRdkContentIOPlugInRhinoPlugInApp initialization

BOOL CSampleRdkContentIOPlugInRhinoPlugInApp::InitInstance()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance initialization belongs here. 
  //   All other significant initialization should take place in
  //   CSampleRdkContentIOPlugInRhinoPlugIn::OnLoadPlugIn().

	CWinApp::InitInstance();

	return TRUE;
}

int CSampleRdkContentIOPlugInRhinoPlugInApp::ExitInstance()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance clean up belongs here. 
  //   All other significant cleanup should take place in either
  //   CSampleRdkContentIOPlugInRhinoPlugIn::OnSaveAllSettings() or CSampleRdkContentIOPlugInRhinoPlugIn::OnUnloadPlugIn().
  return CWinApp::ExitInstance();
}
