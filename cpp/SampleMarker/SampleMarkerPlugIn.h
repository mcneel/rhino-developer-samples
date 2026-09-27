#pragma once

#include "SampleMarkerEventWatcher.h"

class CSampleMarkerPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleMarkerPlugIn();
  ~CSampleMarkerPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  int OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL32 AddToPlugInHelpMenu() const;
  BOOL32 OnDisplayPlugInHelp(HWND hWnd) const;

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  CSampleMarkerEventWatcher m_watcher;
};

CSampleMarkerPlugIn& SampleMarkerPlugIn();



