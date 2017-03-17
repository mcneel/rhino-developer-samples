/////////////////////////////////////////////////////////////////////////////
// RhinoLoadLibrary.cpp
//

#include "StdAfx.h"
#include "RhinoLoadLibrary.h"

CRhinoLoadLibrary::CRhinoLoadLibrary()
: m_hInst(0)
{
}

CRhinoLoadLibrary::CRhinoLoadLibrary( const wchar_t* pszLibName )
: m_hInst(0)
{
  Load( pszLibName );
}

CRhinoLoadLibrary::~CRhinoLoadLibrary()
{
  Free();
}

CRhinoLoadLibrary::operator HINSTANCE() const
{ 
  return m_hInst; 
}

HINSTANCE CRhinoLoadLibrary::Detach()
{ 
  HINSTANCE hInst = m_hInst; 
  m_hInst = 0; 
  return hInst; 
}

bool CRhinoLoadLibrary::Load( const wchar_t* pszLibName, DWORD dwFlags )
{
  Free(); 
  if( pszLibName && pszLibName[0] )
    m_hInst = ::LoadLibraryEx( pszLibName, 0, dwFlags );
  return IsLoaded();
}

void CRhinoLoadLibrary::Free()
{
  if( IsLoaded() )
  {
    ::FreeLibrary( m_hInst );
    m_hInst = 0;
  }
}

bool CRhinoLoadLibrary::IsLoaded() const
{ 
  return ( m_hInst != 0 ); 
}

FARPROC CRhinoLoadLibrary::ProcAddress( const char* pszFuncName ) const
{ 
  if( IsLoaded() )
  {
    if( pszFuncName && pszFuncName[0] )
      return ::GetProcAddress( m_hInst, pszFuncName );
  }
  return 0;
}

bool CRhinoLoadLibrary::ModuleFileName( CString& filename ) const
{
  bool rc = false;
  if( IsLoaded() )
  {
    wchar_t szFileName[MAX_PATH];
    if( ::GetModuleFileName(m_hInst, szFileName, MAX_PATH) )
    {
      filename = szFileName;
      rc = true;
    }
  } 
  return rc;
}

bool CRhinoLoadLibrary::IsDataFile() const
{
  if( IsLoaded() )
    return ( ((ULONG_PTR)(m_hInst)) & (ULONG_PTR)1 ) ? true : false;
  return false;
}

bool CRhinoLoadLibrary::IsImageMapping() const
{
  if( IsLoaded() )
    return ( ((ULONG_PTR)(m_hInst)) & (ULONG_PTR)2 ) ? true : false;
  return false;
}

bool CRhinoLoadLibrary::IsResource() const
{
  if( IsLoaded() )
    return ( IsDataFile() || IsImageMapping() ) ? true : false;
  return false;
}
