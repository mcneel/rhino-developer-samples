#pragma once

class CSampleEventWatcherPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleEventWatcherPlugIn();
  ~CSampleEventWatcherPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  int OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL32 AddToPlugInHelpMenu() const;
  BOOL32 OnDisplayPlugInHelp(HWND hWnd) const;

  CRhinoCommand::result MoveObjects(const CRhinoCommandContext& context);

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleEventWatcherPlugIn& SampleEventWatcherPlugIn();



