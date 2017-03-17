#pragma once

#include "SampleSpaceGizmoWnd.h"
#include "SampleSpaceGizmoSettings.h"

class CSampleSpaceGizmoPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleSpaceGizmoPlugIn();
  ~CSampleSpaceGizmoPlugIn();

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
  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void AddPagesToOptionsDialog(HWND hWnd, ON_SimpleArray<CRhinoOptionsDialogPage*>& pages);

  // SpaceGizmo members
  const CSampleSpaceGizmoSettings& SpaceGizmoSettings() const;
  void SetSpaceGizmoSettings(const CSampleSpaceGizmoSettings& settings);

private:
  BOOL CreateSpaceGizmoWnd();
  void DestroySpaceGizmoWnd();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
  CSampleSpaceGizmoWnd* m_pWnd;
  CSampleSpaceGizmoSettings m_settings;
};

CSampleSpaceGizmoPlugIn& SampleSpaceGizmoPlugIn();



