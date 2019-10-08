#pragma once

// SampleSharedXform library
// Include this in any Rhino C++ project that will access this library using run-time dynamic linking.
// https://docs.microsoft.com/en-us/windows/win32/dlls/using-run-time-dynamic-linking

// CSampleSharedXformTable class declaration
// The transformation table is a dictionary that maps Rhino object ids
// with a collection of 4x4 transforms.

class CSampleSharedXformTable
{
public:
  CSampleSharedXformTable();
  CSampleSharedXformTable(const wchar_t* pszLibName);
  ~CSampleSharedXformTable();

  // Description:
  //   Loads the SampleSharedXform SDK library
  //   Wrapper member for Win32 LoadLibraryEx() function.
  bool Load(const wchar_t* pszLibName, DWORD dwFlags = 0);

  // Description:
  //   Unloads the SampleSharedXform SDK library
  //   Wrapper member for Win32 FreeLibrary() function.
  void Free();

  // Description:
  //   Verifies that a SampleSharedXform SDK library has been loaded.
  bool IsLoaded() const;

public:
  // Description:
  //   Returns the number of Rhino objects in the transformation table.
  int Count() const;

  // Description:
  //   Verifies a Rhino object that is in the transformation table.
  // Parameters:
  //   uuid [in] - The id of the Rhino object.
  // Returns:
  //   true if the Rhino object exists in the transformation table, false otherwise.
  bool Contains(ON_UUID uuid) const;

  // Description:
  //   Returns the ids of the Rhino objects in the transformation table.
  // Parameters:
  //   uuids [out] - The id of the Rhino object in the transformation table.
  // Returns:
  //   The number of ids in uuids.
  int Ids(ON_SimpleArray<ON_UUID>& uuids) const;

  // Description:
  //   Returns the runtime serial number of the transformations for a Rhino object.
  //   Use this value to determine if the object's transformations have changed.
  // Parameters:
  //   uuid [in] - The id of the Rhino object.
  // Returns:
  //   The runtime serial number of the Rhino object's transforms, 0 otherwise.
  unsigned int RuntimeSerialNumber(ON_UUID uuid) const;

  // Description:
  //   Returns the 4x4 transforms for a Rhino object.
  // Parameters:
  //   uuid   [in]  - The id of the Rhino object.
  //   xforms [out] - The 4x4 transforms.
  // Returns:
  //   The number of transforms in xforms.
  int Transforms(ON_UUID uuid, ON_SimpleArray<ON_Xform>& xforms) const;

private:
  typedef int          (__stdcall *ON_XFORMTABLE_COUNT)();
  typedef bool         (__stdcall *ON_XFORMTABLE_CONTAINS)(ON_UUID);
  typedef int          (__stdcall *ON_XFORMTABLE_IDS)(ON_SimpleArray<ON_UUID>*);
  typedef unsigned int (__stdcall *ON_XFORMTABLE_RUNTIMESERIALNUMBER)(ON_UUID);
  typedef int          (__stdcall *ON_XFORMTABLE_TRANSFORMS)(ON_UUID, ON_SimpleArray<ON_Xform>*);

  ON_XFORMTABLE_COUNT m_XformTableCount;
  ON_XFORMTABLE_CONTAINS m_XformTableContains;
  ON_XFORMTABLE_IDS m_XformTableIds;
  ON_XFORMTABLE_RUNTIMESERIALNUMBER m_XformTableRuntimeSerialNumber;
  ON_XFORMTABLE_TRANSFORMS m_XformTableTransforms;

private:
  CSampleSharedXformTable(const CSampleSharedXformTable&) = delete;;
  CSampleSharedXformTable& operator=(const CSampleSharedXformTable&) = delete;;
  HINSTANCE m_hInstance;
};
