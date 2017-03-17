/////////////////////////////////////////////////////////////////////////////
// SampleOptionsListCtrlPlugIn.h : main header file for the SampleOptionsListCtrl plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleOptionsListCtrlPlugIn
// See SampleOptionsListCtrlPlugIn.cpp for the implementation of this class
//

class CSampleOptionsListCtrlPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleOptionsListCtrlPlugIn();
  ~CSampleOptionsListCtrlPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleOptionsListCtrlPlugIn& SampleOptionsListCtrlPlugIn();



