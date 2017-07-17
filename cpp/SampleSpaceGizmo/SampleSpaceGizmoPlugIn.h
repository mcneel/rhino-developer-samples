#pragma once

#include "SampleSpaceGizmoWnd.h"
#include "SampleSpaceGizmoSettings.h"

class CSampleSpaceGizmoPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleSpaceGizmoPlugIn();
  ~CSampleSpaceGizmoPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const override;
  const wchar_t* PlugInVersion() const override;
  GUID PlugInID() const override;
  BOOL OnLoadPlugIn() override;
  void OnUnloadPlugIn() override;

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const override;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const override;

  // Additional overrides
  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc) override;
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc) override;
  void AddPagesToOptionsDialog(CRhinoOptionsPageCollection& collection) override;

  // SpaceGizmo members
  const CSampleSpaceGizmoSettings& SpaceGizmoSettings() const;
  void SetSpaceGizmoSettings(const CSampleSpaceGizmoSettings& settings);

private:
  BOOL CreateSpaceGizmoWnd();
  void DestroySpaceGizmoWnd();

private:
  ON_wString m_plugin_version;
  // Additional class information
  CSampleSpaceGizmoWnd* m_pWnd;
  CSampleSpaceGizmoSettings m_settings;
};

CSampleSpaceGizmoPlugIn& SampleSpaceGizmoPlugIn();
