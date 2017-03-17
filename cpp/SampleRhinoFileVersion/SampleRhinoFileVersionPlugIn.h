#pragma once

class CSampleRhinoFileVersionWatcher : public CRhinoEventWatcher
{
  void OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference);
};

class CSampleRhinoFileVersionPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleRhinoFileVersionPlugIn();
  ~CSampleRhinoFileVersionPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  CRhinoPlugIn::plugin_load_time PlugInLoadTime();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  CSampleRhinoFileVersionWatcher m_watcher;
};

CSampleRhinoFileVersionPlugIn& SampleRhinoFileVersionPlugIn();



