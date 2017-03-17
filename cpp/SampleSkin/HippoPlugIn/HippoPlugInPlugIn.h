/////////////////////////////////////////////////////////////////////////////
// HippoPlugInPlugIn.h : main header file for the HippoPlugIn plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CHippoPlugInPlugIn
// See HippoPlugInPlugIn.cpp for the implementation of this class
//

class CHippoPlugInPlugIn : public CRhinoUtilityPlugIn
{
public:
  CHippoPlugInPlugIn();
  ~CHippoPlugInPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp( HWND hWnd ) const;

  // Additional overrides required for skinning
  void OnInitPlugInMenuPopups( WPARAM wparam, LPARAM lparam );
  BOOL OnPlugInMenuCommand( WPARAM wparam );
  plugin_load_time PlugInLoadTime();

private:
  ON_wString m_plugin_version;
};

CHippoPlugInPlugIn& HippoPlugInPlugIn();



