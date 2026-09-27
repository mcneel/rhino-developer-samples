#pragma once

class CSampleRhinoFileVersionWatcher : public CRhinoEventWatcher
{
  void OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL32 bMerge, BOOL32 bReference);
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
  int OnLoadPlugIn();
  void OnUnloadPlugIn();

  CRhinoPlugIn::plugin_load_time PlugInLoadTime();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  CSampleRhinoFileVersionWatcher m_watcher;
};

CSampleRhinoFileVersionPlugIn& SampleRhinoFileVersionPlugIn();



