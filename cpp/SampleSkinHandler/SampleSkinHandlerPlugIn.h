#pragma once

class CSampleSkinHandlerPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleSkinHandlerPlugIn();
  ~CSampleSkinHandlerPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  // Additional overrides required for skinning
  void OnInitPlugInMenuPopups(WPARAM wparam, LPARAM lparam);
  BOOL OnPlugInMenuCommand(WPARAM wparam);
  CRhinoPlugIn::plugin_load_time PlugInLoadTime();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleSkinHandlerPlugIn& SampleSkinHandlerPlugIn();



