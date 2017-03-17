/////////////////////////////////////////////////////////////////////////////
// RhinoLoadLibrary.h
//

#pragma once

class CRhinoLoadLibrary
{
public:
	CRhinoLoadLibrary();
	CRhinoLoadLibrary( const wchar_t* pszLibName );
	~CRhinoLoadLibrary();

  /*
  Description:
    Wrapper member for Win32 LoadLibraryEx() function.
  */
  bool Load( const wchar_t* pszLibName, DWORD dwFlags = 0 );

  /*
  Description:
    Wrapper member for Win32 FreeLibrary() function.
  */
  void Free();

  /*
  Description:
    Verifies that a module has been loaded.
  */
	bool IsLoaded() const;

  /*
  Description:
    Wrapper member for Win32 GetProcAddress() function.
  */
	FARPROC ProcAddress( const char* pszFuncName ) const;

  /*
  Description:
    Wrapper member for Win32 GetModuleFileName() function.
  */
  bool ModuleFileName( CString& filename ) const;

  /*
  Description:
    Cast operator, returns the handle to the loaded module.
  */
	operator HINSTANCE() const;

  /*
  Description:
    Detaches the handle to the loaded module. Caller responsible
    for calling FreeLibrary on returned instance handle.
  */
  HINSTANCE Detach(); 

  /*
  Description:
    Returns true if the module was loaded with 
    LOAD_LIBRARY_AS_DATAFILE or LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE.
  */
  bool IsDataFile() const;

  /*
  Description:
    Returns true if the module was loaded with 
    LOAD_LIBRARY_AS_IMAGE_RESOURCE.
  */
  bool IsImageMapping() const;

  /*
  Description:
    Returns true if the module was loaded with 
    LOAD_LIBRARY_AS_IMAGE_RESOURCE or either 
    LOAD_LIBRARY_AS_DATAFILE or LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE.
  */
  bool IsResource() const;

private:
  // no copy constructor
	CRhinoLoadLibrary( const CRhinoLoadLibrary& );
  // no operator=
	CRhinoLoadLibrary& operator=( const CRhinoLoadLibrary& );

private:
	HINSTANCE m_hInst; // handle to the loaded module
};