/////////////////////////////////////////////////////////////////////////////
// SampleObjectUserDataPlugIn.h : main header file for the SampleObjectUserData plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleObjectUserDataPlugIn
// See SampleObjectUserDataPlugIn.cpp for the implementation of this class
//

class CSampleObjectUserDataPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleObjectUserDataPlugIn();
  ~CSampleObjectUserDataPlugIn();

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

CSampleObjectUserDataPlugIn& SampleObjectUserDataPlugIn();



