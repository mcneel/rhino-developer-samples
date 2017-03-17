#include "stdafx.h"
#include "SampleAutomationApp.h"

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

// CSampleAutomationApp

BEGIN_MESSAGE_MAP(CSampleAutomationApp, CWinApp)
END_MESSAGE_MAP()

// CSampleAutomationApp construction

CSampleAutomationApp::CSampleAutomationApp()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance construction belongs here. 
  //   All other significant initialization should take place in
  //   CSampleAutomationPlugIn::OnLoadPlugIn().
}

// The one and only CSampleAutomationApp object
CSampleAutomationApp theApp;

const GUID CDECL _tlid = { 0x441AB19B, 0xF8DF, 0x45C4, { 0x83, 0x31, 0xE4, 0xCC, 0x16, 0xE, 0xAA, 0x89 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

// CSampleAutomationApp initialization

BOOL CSampleAutomationApp::InitInstance()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance initialization belongs here. 
  //   All other significant initialization should take place in
  //   CSampleAutomationPlugIn::OnLoadPlugIn().

  CWinApp::InitInstance();

  // Register all OLE server (factories) as running.  This enables the
  //  OLE libraries to create objects from other applications.
  COleObjectFactory::RegisterAll();

  return TRUE;
}

int CSampleAutomationApp::ExitInstance()
{
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  //   Only standard MFC DLL instance clean up belongs here. 
  //   All other significant cleanup should take place in either
  //   CSampleAutomationPlugIn::OnSaveAllSettings() or CSampleAutomationPlugIn::OnUnloadPlugIn().
  return CWinApp::ExitInstance();
}

// DllGetClassObject - Returns class factory
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return AfxDllGetClassObject(rclsid, riid, ppv);
}

// DllCanUnloadNow - Allows COM to unload DLL
STDAPI DllCanUnloadNow(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return AfxDllCanUnloadNow();
}

// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
    return SELFREG_E_TYPELIB;

  if (!COleObjectFactory::UpdateRegistryAll())
    return SELFREG_E_CLASS;

  return S_OK;
}

// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
    return SELFREG_E_TYPELIB;

  if (!COleObjectFactory::UpdateRegistryAll(FALSE))
    return SELFREG_E_CLASS;

  return S_OK;
}
