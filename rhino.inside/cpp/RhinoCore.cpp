#include "pch.h"
#include "RhinoCore.h"

// https://learn.microsoft.com/en-us/cpp/build/reference/understanding-the-helper-function
static FARPROC WINAPI delayHookRhinoLibrary(unsigned dliNotify, PDelayLoadInfo pdli)
{
  static const wchar_t* RhinoLibraryPath = L"C:\\Program Files\\Rhino 8\\System\\RhinoLibrary.dll";
  if (dliNotify == dliNotePreLoadLibrary && pdli && _stricmp(pdli->szDll, "RhinoLibrary.dll") == 0)
    return (FARPROC)LoadLibraryEx(RhinoLibraryPath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
  return nullptr; 
}

static const PfnDliHook __pfnDliNotifyHook2 = delayHookRhinoLibrary;

// Exported from RhinoCore.dll
extern "C" HRESULT StartupInProcess(int argc, wchar_t** argv, const STARTUPINFO* pStartUpInfo, HWND hHostWnd);
extern "C" HRESULT ShutdownInProcess();

CRhinoCore::CRhinoCore(int argc, wchar_t** argv)
{
  StartupInProcess(argc, argv, nullptr, HWND_DESKTOP);
}

CRhinoCore::~CRhinoCore()
{
  ShutdownInProcess();
}

