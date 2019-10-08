#include "StdAfx.h"
#include "SampleSharedXformSdk.h"

// SampleSharedXform library
// Include this in any Rhino C++ project that will access this library using run-time dynamic linking.
// https://docs.microsoft.com/en-us/windows/win32/dlls/using-run-time-dynamic-linking

// CSampleSharedXformTable class definition

CSampleSharedXformTable::CSampleSharedXformTable()
  : m_hInstance(0)
  , m_XformTableCount(nullptr)
  , m_XformTableContains(nullptr)
  , m_XformTableIds(nullptr)
  , m_XformTableRuntimeSerialNumber(nullptr)
  , m_XformTableTransforms(nullptr)
{
}

CSampleSharedXformTable::CSampleSharedXformTable(const wchar_t* pszLibName)
  : m_hInstance(0)
{
  Load(pszLibName);
}

CSampleSharedXformTable::~CSampleSharedXformTable()
{
  Free();
}

bool CSampleSharedXformTable::Load(const wchar_t* pszLibName, DWORD dwFlags)
{
  Free();
  if (pszLibName && pszLibName[0])
  {
    m_hInstance = ::LoadLibraryEx(pszLibName, 0, dwFlags);
    if (m_hInstance != 0)
    {
      m_XformTableCount = (ON_XFORMTABLE_COUNT)GetProcAddress(m_hInstance, "ON_XformTable_Count");
      m_XformTableContains = (ON_XFORMTABLE_CONTAINS)GetProcAddress(m_hInstance, "ON_XformTable_Contains");
      m_XformTableIds = (ON_XFORMTABLE_IDS)GetProcAddress(m_hInstance, "ON_XformTable_Ids");
      m_XformTableRuntimeSerialNumber = (ON_XFORMTABLE_RUNTIMESERIALNUMBER)GetProcAddress(m_hInstance, "ON_XformTable_RuntimeSerialNumber");
      m_XformTableTransforms = (ON_XFORMTABLE_TRANSFORMS)GetProcAddress(m_hInstance, "ON_XformTable_Transforms");
    }
  }
  return IsLoaded();
}

void CSampleSharedXformTable::Free()
{
  if (IsLoaded())
  {
    ::FreeLibrary(m_hInstance);
    m_hInstance = 0;
    m_XformTableCount = nullptr;
    m_XformTableContains = nullptr;
    m_XformTableIds = nullptr;
    m_XformTableRuntimeSerialNumber = nullptr;
    m_XformTableTransforms = nullptr;
  }
}

bool CSampleSharedXformTable::IsLoaded() const
{
  if ( m_hInstance 
    && m_XformTableCount 
    && m_XformTableContains 
    && m_XformTableIds 
    && m_XformTableRuntimeSerialNumber 
    && m_XformTableTransforms
    )
    return true;
  return false;
}

int CSampleSharedXformTable::Count() const
{
  if (!IsLoaded())
    return 0;
  return m_XformTableCount();
}

bool CSampleSharedXformTable::Contains(ON_UUID uuid) const
{
  if (!IsLoaded())
    return false;
  return m_XformTableContains(uuid);
}

int CSampleSharedXformTable::Ids(ON_SimpleArray<ON_UUID>& uuids) const
{
  if (!IsLoaded())
    return 0;
  return m_XformTableIds(&uuids);
}

unsigned int CSampleSharedXformTable::RuntimeSerialNumber(ON_UUID uuid) const
{
  if (!IsLoaded())
    return 0;
  return m_XformTableRuntimeSerialNumber(uuid);
}

int CSampleSharedXformTable::Transforms(ON_UUID uuid, ON_SimpleArray<ON_Xform>& xforms) const
{
  if (!IsLoaded())
    return 0;
  return m_XformTableTransforms(uuid, &xforms);
}
