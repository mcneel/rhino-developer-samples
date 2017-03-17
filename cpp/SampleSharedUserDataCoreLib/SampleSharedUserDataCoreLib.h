#pragma once

#if defined(SHAREDDATACORELIB_DLL_EXPORTS)

#pragma message( " --- Compiling SampleSharedUserDataCoreLib as a Windows DLL" )
#define SHAREDDATACORELIB_CLASS __declspec(dllexport)
#define SHAREDDATACORELIB_FUNCTION __declspec(dllexport)
#define SHAREDDATACORELIB_DATA __declspec(dllexport)

#else

#pragma message( " --- Using SampleSharedUserDataCoreLib as a Windows DLL" )
#define SHAREDDATACORELIB_CLASS __declspec(dllimport)
#define SHAREDDATACORELIB_FUNCTION __declspec(dllimport)
#define SHAREDDATACORELIB_DATA __declspec(dllimport)

#endif

/////////////////////////////////////////////////////////////////////////////
// Exported functions

SHAREDDATACORELIB_FUNCTION
bool HasPlugInUserData(const CRhinoObject* object);

SHAREDDATACORELIB_FUNCTION
bool AddPlugInUserData(CRhinoDoc& doc, const CRhinoObject* object, ON_3dPoint point, ON_wString string);

SHAREDDATACORELIB_FUNCTION
bool SetPlugInUserData(CRhinoDoc& doc, const CRhinoObject* object, ON_3dPoint point, ON_wString string);

SHAREDDATACORELIB_FUNCTION
bool GetPlugInUserData(const CRhinoObject* object, ON_3dPoint& point, ON_wString& string);

SHAREDDATACORELIB_FUNCTION
bool RemovePlugInUserData(CRhinoDoc& doc, const CRhinoObject* object);

/////////////////////////////////////////////////////////////////////////////
// Plug-in linking pragmas

#if !defined(SHAREDDATACORELIB_DLL_EXPORTS)

#if defined(NDEBUG) && defined(RHINO_DEBUG_PLUGIN)
#pragma message( " --- Plug-in linking with SampleSharedUserDataCoreLib Debug x64." )
#pragma comment(lib, "../x64/Debug/SampleSharedUserDataCoreLib.lib")

#elif defined(NDEBUG)
#pragma message( " --- Plug-in linking with SampleSharedUserDataCoreLib Release x64." )
#pragma comment(lib, "../x64/Release/SampleSharedUserDataCoreLib.lib")

#else
#pragma message( " --- Plug-in linking with SampleSharedUserDataCoreLib DebugRhino x64." )
#pragma comment(lib, "../x64/DebugRhino/SampleSharedUserDataCoreLib.lib")
#endif

#endif // SHAREDDATACORELIB_DLL_EXPORTS