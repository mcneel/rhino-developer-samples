#pragma once

#include "SampleCommandsEventWatcher.h"

class CSampleCommandsPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleCommandsPlugIn();
  ~CSampleCommandsPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  // Document user data overrides
  BOOL CallWriteDocument(const CRhinoFileWriteOptions&);
  BOOL WriteDocument(CRhinoDoc&, ON_BinaryArchive&, const CRhinoFileWriteOptions&);
  BOOL ReadDocument(CRhinoDoc&, ON_BinaryArchive&, const CRhinoFileReadOptions&);

  // Used by SampleDocumentUserData
  int StringTableCount() const;
  ON_wString GetStringTableItem(int);
  int FindStringTableItem(const wchar_t*);
  int AddStringTableItem(const wchar_t*);
  bool DeleteStringTableItem(const wchar_t*);
  void ClearStringTable();

  // Used by SampleObjectCursor and SamplePointCursor
  HCURSOR SampleCursor();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  // Used by SampleDocumentUserData
  ON_ClassArray<ON_wString> m_string_table;
  CSampleCommandsEventWatcher m_watcher;

  // Used by SampleObjectCursor and SamplePointCursor
  HCURSOR m_hCursor;
};

CSampleCommandsPlugIn& SampleCommandsPlugIn();



