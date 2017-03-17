/////////////////////////////////////////////////////////////////////////////
// PlugInCoreLib.h
//

#pragma once

#if defined(PLUGINCORELIB_DLL_EXPORTS)

#pragma message( " --- Compiling PlugInCoreLib as a Windows DLL" )
#define PLUGINCORELIB_CLASS __declspec(dllexport)
#define PLUGINCORELIB_FUNCTION __declspec(dllexport)
#define PLUGINCORELIB_DATA __declspec(dllexport)

#else

#pragma message( " --- Using PlugInCoreLib as a Windows DLL" )
#define PLUGINCORELIB_CLASS __declspec(dllimport)
#define PLUGINCORELIB_FUNCTION __declspec(dllimport)
#define PLUGINCORELIB_DATA __declspec(dllimport)

#endif

/////////////////////////////////////////////////////////////////////////////
// Exported functions

PLUGINCORELIB_FUNCTION
bool HasPlugInUserData( const CRhinoObject* object );

PLUGINCORELIB_FUNCTION
bool AddPlugInUserData( CRhinoDoc& doc, const CRhinoObject* object, ON_3dPoint point, ON_wString string );

PLUGINCORELIB_FUNCTION
bool SetPlugInUserData( CRhinoDoc& doc, const CRhinoObject* object, ON_3dPoint point, ON_wString string );

PLUGINCORELIB_FUNCTION
bool GetPlugInUserData( const CRhinoObject* object, ON_3dPoint& point, ON_wString& string );

PLUGINCORELIB_FUNCTION
bool RemovePlugInUserData( CRhinoDoc& doc, const CRhinoObject* object );

/////////////////////////////////////////////////////////////////////////////
// Plug-in linking pragmas

#if !defined(PLUGINCORELIB_DLL_EXPORTS)

#if defined(WIN64)

#if defined(NDEBUG) && defined(RHINO_DEBUG_PLUGIN)
#pragma message( " --- Plug-in linking with PlugInCoreLib PseudoDebug x64." )
#pragma comment(lib, "../x64/PseudoDebug/PlugInCoreLib.lib")

#elif defined(NDEBUG)
#pragma message( " --- Plug-in linking with PlugInCoreLib Release x64." )
#pragma comment(lib, "../x64/Release/PlugInCoreLib.lib")

#else
#pragma message( " --- Plug-in linking with PlugInCoreLib Debug x64." )
#pragma comment(lib, "../x64/Debug/PlugInCoreLib.lib")
#endif

#else

#if defined(NDEBUG) && defined(RHINO_DEBUG_PLUGIN)
#pragma message( " --- Plug-in linking with PlugInCoreLib PseudoDebug Win32." )
#pragma comment(lib, "../PseudoDebug/PlugInCoreLib.lib")

#elif defined(NDEBUG)
#pragma message( " --- Plug-in linking with PlugInCoreLib Release Win32." )
#pragma comment(lib, "../Release/PlugInCoreLib.lib")

#else
#pragma message( " --- Plug-in linking with PlugInCoreLib Debug Win32." )
#pragma comment(lib, "../Debug/PlugInCoreLib.lib")
#endif

#endif // WIN64

#endif // PLUGINCORELIB_DLL_EXPORTS