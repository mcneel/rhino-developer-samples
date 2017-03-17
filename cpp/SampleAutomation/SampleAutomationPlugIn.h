#pragma once

#include "SampleScriptObject.h"

class CSampleAutomationPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleAutomationPlugIn();
  ~CSampleAutomationPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  // Additional overrides
  CRhinoPlugIn::plugin_load_time PlugInLoadTime();
  LPUNKNOWN GetPlugInObjectInterface(const ON_UUID& iid);

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  CSampleScriptObject m_script_object;
};

CSampleAutomationPlugIn& SampleAutomationPlugIn();



