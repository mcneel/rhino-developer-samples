#pragma once

// SampleSharedXform library
// Include this in any Rhino C++ project that will access this library using load-time dynamic linking. 
// https://docs.microsoft.com/en-us/windows/win32/dlls/using-load-time-dynamic-linking

#if defined(DLL_EXPORTS)
#define DLL_CPP_CLASS __declspec(dllexport)
#define DLL_CPP_FUNCTION __declspec(dllexport)
#define DLL_CPP_DATA __declspec(dllexport)
#define DLL_C_FUNCTION extern "C" __declspec(dllexport)
#else
#define DLL_CPP_CLASS __declspec(dllimport)
#define DLL_CPP_FUNCTION __declspec(dllimport)
#define DLL_CPP_DATA __declspec(dllimport)
#define DLL_C_FUNCTION extern "C" __declspec(dllimport)
#pragma message( " --- Linking with SampleSharedXformLib Release x64" )
#if defined(RHINO_DEBUG_PLUGIN)
#pragma comment( lib, "../../x64/Debug/SampleSharedXformLib.lib" )
#else
#pragma comment( lib, "../../x64/Release/SampleSharedXformLib.lib" )
#endif
#endif

// CSampleSharedXformTable class declaration
// The transformation table is a dictionary that maps Rhino object ids
// with a collection of 4x4 transforms.

class DLL_CPP_CLASS CSampleSharedXformTable
{
public:
  // Description:
  //   Returns the number of Rhino objects in the transformation table.
  static int Count();

  // Description:
  //   Verifies a Rhino object that is in the transformation table.
  // Parameters:
  //   uuid [in] - The id of the Rhino object.
  // Returns:
  //   true if the Rhino object exists in the transformation table, false otherwise.
  static bool Contains(ON_UUID uuid);

  // Description:
  //   Returns the ids of the Rhino objects in the transformation table.
  // Parameters:
  //   uuids [out] - The id of the Rhino object in the transformation table.
  // Returns:
  //   The number of ids in uuids.
  static int Ids(ON_SimpleArray<ON_UUID>& uuids);

  // Description:
  //   Returns the runtime serial number of the transformations for a Rhino object.
  //   Use this value to determine if the object's transformations have changed.
  // Parameters:
  //   uuid [in] - The id of the Rhino object.
  // Returns:
  //   The runtime serial number of the Rhino object's transforms, 0 otherwise.
  static unsigned int RuntimeSerialNumber(ON_UUID uuid);

  // Description:
  //   Returns the 4x4 transforms for a Rhino object.
  // Parameters:
  //   uuid   [in]  - The id of the Rhino object.
  //   xforms [out] - The 4x4 transforms.
  // Returns:
  //   The number of transforms in xforms.
  static int Transforms(ON_UUID uuid, ON_SimpleArray<ON_Xform>& xforms);
};
