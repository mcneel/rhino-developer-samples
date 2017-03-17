/////////////////////////////////////////////////////////////////////////////
// SampleTabDialogPlugIn.h : main header file for the SampleTabDialog plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleTabDialogPlugIn
// See SampleTabDialogPlugIn.cpp for the implementation of this class
//

class CSampleTabDialogPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleTabDialogPlugIn();
  ~CSampleTabDialogPlugIn();

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

CSampleTabDialogPlugIn& SampleTabDialogPlugIn();



