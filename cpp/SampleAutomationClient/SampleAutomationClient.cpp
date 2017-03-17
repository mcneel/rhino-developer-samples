#include "stdafx.h"
#include "SampleAutomationClient.h"
#include "RhinoApplication.h"
#include "RhinoInterface.h"
#include "SampleScriptObject.h"

// The one and only application object
CWinApp theApp;

using namespace std;

static CString SampleAutomationPlugInID()
{
  // From ../SampleAutomation/SampleAutomationPlugIn.cpp
  CString str(L"50CB06F5-1E78-4D57-A82F-1606ED35B7C");
  return str;
}

static CString SampleScriptObjectID()
{
  // From ../SampleAutomation/SampleScriptObject.cpp
  CString str(L"52D524B4-C92F-412E-AAD4-56B879EBAF68");
  return str;
}

int wmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);
  UNREFERENCED_PARAMETER(envp);

  HMODULE hModule = ::GetModuleHandle(0);
  if (nullptr == hModule)
  {
    wprintf(L"Fatal Error: GetModuleHandle failed\n");
    return 1;
  }

  if (!AfxWinInit(hModule, 0, ::GetCommandLine(), 0))
  {
    wprintf(L"Fatal Error: MFC initialization failed\n");
    return 1;
  }

  CoInitializeEx(0, COINIT_APARTMENTTHREADED);

  //CRhinoApplication app;
  //BOOL rc = app.CreateDispatch(L"Rhino.Application", 0);

  CRhinoInterface app;
  BOOL rc = app.CreateDispatch(L"Rhino.Interface", 0);

  if (!rc)
  {
    CoUninitialize();
    wprintf(L"Fatal Error: Rhino object creation failed\n");
    return 1;
  }

  const int bail_milliseconds = 15 * 1000;
  int time_waiting = 0;
  while (0 == app.IsInitialized())
  {
    Sleep(100);
    time_waiting += 100;
    if (time_waiting > bail_milliseconds)
    {
      CoUninitialize();
      wprintf(L"Fatal Error: Rhino initialization failed\n");
      return 1;
    }
  }

  LPUNKNOWN lpUnknown = app.GetPlugInObject(SampleAutomationPlugInID(), SampleScriptObjectID());
  if (0 == lpUnknown)
  {
    app.ReleaseDispatch();
    CoUninitialize();
    wprintf(L"Fatal Error: SampleScriptObject creation failed\n");
    return 1;
  }

  LPDISPATCH lpDispatch = 0;
  HRESULT hr = lpUnknown->QueryInterface(IID_IDispatch, (void**)&lpDispatch);
  if (FAILED(hr))
  {
    lpUnknown->Release();
    app.ReleaseDispatch();
    CoUninitialize();
    wprintf(L"Fatal Error: SampleRhino object creation failed\n");
    return 1;
  }

  CSampleScriptObject plugin;
  plugin.AttachDispatch(lpDispatch, TRUE);

  CString str = plugin.Hello();
  wprintf(L"%s\n", (const wchar_t*)str);

  plugin.ReleaseDispatch();
  app.ReleaseDispatch();

  CoUninitialize();

  return 0;
}
