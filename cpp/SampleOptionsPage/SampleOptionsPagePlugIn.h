/////////////////////////////////////////////////////////////////////////////
// SampleOptionsPagePlugIn.h : main header file for the SampleOptionsPage plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleOptionsPagePlugIn
// See SampleOptionsPagePlugIn.cpp for the implementation of this class
//

class CSampleOptionsPagePlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleOptionsPagePlugIn();
  ~CSampleOptionsPagePlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Additional overrides
  CRhinoPlugIn::plugin_load_time PlugInLoadTime();
  void AddPagesToOptionsDialog( HWND hWnd, ON_SimpleArray<CRhinoOptionsDialogPage*>& pages );
  void AddPagesToDocumentPropertiesDialog( CRhinoDoc& doc, HWND hWnd, ON_SimpleArray<CRhinoOptionsDialogPage*>& pages );

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleOptionsPagePlugIn& SampleOptionsPagePlugIn();



