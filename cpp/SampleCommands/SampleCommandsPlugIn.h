#pragma once

#include "SampleDocumentUserData.h"
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
  ON_wString GetStringTableItem(int) const;
  int FindStringTableItem(const wchar_t*)const;
  int AddStringTableItem(const wchar_t*);
  bool DeleteStringTableItem(const wchar_t*);
  void ClearStringTable();

  // Used by SampleObjectCursor and SamplePointCursor
  HCURSOR SampleCursor();

private:
  ON_wString m_plugin_version;

  // Used by SampleDocumentUserData
  CSampleDocumentUserData m_string_table;

  // Event watcher
  CSampleCommandsEventWatcher m_watcher;

  // Used by SampleObjectCursor and SamplePointCursor
  HCURSOR m_hCursor;
};

CSampleCommandsPlugIn& SampleCommandsPlugIn();



