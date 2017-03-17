#pragma once

class CSampleLocalizationPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleLocalizationPlugIn();
  ~CSampleLocalizationPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleLocalizationPlugIn& SampleLocalizationPlugIn();



