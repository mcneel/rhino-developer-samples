/////////////////////////////////////////////////////////////////////////////
// SampleDocumentUserDataPlugIn.h : main header file for the SampleDocumentUserData plug-in
//

#pragma once

#include "SampleDocumentUserDataEventWatcher.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleDocumentUserDataPlugIn
// See SampleDocumentUserDataPlugIn.cpp for the implementation of this class
//

class CSampleDocumentUserDataPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleDocumentUserDataPlugIn();
  ~CSampleDocumentUserDataPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Document user data overrides
  BOOL CallWriteDocument( const CRhinoFileWriteOptions& );
  BOOL WriteDocument( CRhinoDoc&, ON_BinaryArchive&, const CRhinoFileWriteOptions& );
  BOOL ReadDocument( CRhinoDoc&, ON_BinaryArchive&, const CRhinoFileReadOptions& );

  int StringTableCount() const;
  ON_wString GetStringTableItem( int );
  int FindStringTableItem( const wchar_t* );
  int AddStringTableItem( const wchar_t* );
  bool DeleteStringTableItem( const wchar_t* );
  void ClearStringTable();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
  ON_ClassArray<ON_wString> m_string_table;

  CSampleDocumentUserDataEventWatcher m_watcher;
};

CSampleDocumentUserDataPlugIn& SampleDocumentUserDataPlugIn();



