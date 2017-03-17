#include "StdAfx.h"
#include "SampleLoadLibrary.h"

CSampleLoadLibrary::CSampleLoadLibrary()
  : m_hModule(0)
{
}

CSampleLoadLibrary::CSampleLoadLibrary(const wchar_t* pszLibName)
  : m_hModule(0)
{
  Load(pszLibName);
}

CSampleLoadLibrary::~CSampleLoadLibrary()
{
  Free();
}

CSampleLoadLibrary::operator HMODULE() const
{
  return m_hModule;
}

HINSTANCE CSampleLoadLibrary::Detach()
{
  HINSTANCE hModule = m_hModule;
  m_hModule = 0;
  return hModule;
}

bool CSampleLoadLibrary::Load(const wchar_t* pszLibName, DWORD dwFlags)
{
  Free();
  if (pszLibName && pszLibName[0])
    m_hModule = ::LoadLibraryEx(pszLibName, 0, dwFlags);
  return IsLoaded();
}

void CSampleLoadLibrary::Free()
{
  if (IsLoaded())
  {
    ::FreeLibrary(m_hModule);
    m_hModule = 0;
  }
}

bool CSampleLoadLibrary::IsLoaded() const
{
  return (m_hModule != 0);
}

FARPROC CSampleLoadLibrary::ProcAddress(const char* pszFuncName) const
{
  if (IsLoaded())
  {
    if (pszFuncName && pszFuncName[0])
      return ::GetProcAddress(m_hModule, pszFuncName);
  }
  return 0;
}

bool CSampleLoadLibrary::ModuleFileName(CString& filename) const
{
  bool rc = false;
  if (IsLoaded())
  {
    wchar_t szFileName[MAX_PATH];
    if (::GetModuleFileName(m_hModule, szFileName, MAX_PATH))
    {
      filename = szFileName;
      rc = true;
    }
  }
  return rc;
}

bool CSampleLoadLibrary::IsDataFile() const
{
  if (IsLoaded())
    return (((ULONG_PTR)(m_hModule)) & (ULONG_PTR)1) ? true : false;
  return false;
}

bool CSampleLoadLibrary::IsImageMapping() const
{
  if (IsLoaded())
    return (((ULONG_PTR)(m_hModule)) & (ULONG_PTR)2) ? true : false;
  return false;
}

bool CSampleLoadLibrary::IsResource() const
{
  if (IsLoaded())
    return (IsDataFile() || IsImageMapping()) ? true : false;
  return false;
}
