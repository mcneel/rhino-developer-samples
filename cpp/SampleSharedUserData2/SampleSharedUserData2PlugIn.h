#pragma once

class CSampleSharedUserData2PlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleSharedUserData2PlugIn();
  ~CSampleSharedUserData2PlugIn();

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

CSampleSharedUserData2PlugIn& SampleSharedUserData2PlugIn();



