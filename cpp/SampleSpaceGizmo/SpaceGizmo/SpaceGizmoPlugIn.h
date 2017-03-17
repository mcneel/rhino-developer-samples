/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoPlugIn.h

#pragma once

#include "SpaceGizmoWnd.h"
#include "SpaceGizmoSettings.h"

// Description:
//   Space Gizmo plug-in

class CSpaceGizmoPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSpaceGizmoPlugIn();
  ~CSpaceGizmoPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* LocalPlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Additional overrides
  void LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
  void SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
  void AddPagesToOptionsDialog( HWND hWnd, ON_SimpleArray<CRhinoOptionsDialogPage*>& pages );

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp( HWND hWnd ) const;

  // SpaceGizmo members
  const CSpaceGizmoSettings& SpaceGizmoSettings() const;
  void SetSpaceGizmoSettings( const CSpaceGizmoSettings& settings );

private:
  BOOL CreateSpaceGizmoWnd();
  void DestroySpaceGizmoWnd();

private:
  ON_wString m_plugin_version;
  CSpaceGizmoWnd* m_pWnd;
  CSpaceGizmoSettings m_settings;
};

CSpaceGizmoPlugIn& SpaceGizmoPlugIn();



