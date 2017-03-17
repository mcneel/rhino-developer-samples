/////////////////////////////////////////////////////////////////////////////
// SampleMenuPlugIn.h : main header file for the SampleMenu plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleMenuPlugIn
// See SampleMenuPlugIn.cpp for the implementation of this class
//

class CSampleMenuPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleMenuPlugIn();
  ~CSampleMenuPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp( HWND hWnd ) const;

  // Additional overrides
  void OnInitPlugInMenuPopups( WPARAM wParam, LPARAM lParam );
  BOOL OnPlugInMenuCommand( WPARAM wParam );

  // Plug-in specific menu helpers
  BOOL IsSampleMenuVisible() const;
  BOOL ShowSampleMenu();
  BOOL HideSampleMenu();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
  CMenu m_menu;
  bool m_bScriptMode;
};

CSampleMenuPlugIn& SampleMenuPlugIn();
