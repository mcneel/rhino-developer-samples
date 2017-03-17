/////////////////////////////////////////////////////////////////////////////
// SampleAttributeUserDataPlugIn.h : main header file for the SampleAttributeUserData plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleAttributeUserDataPlugIn
// See SampleAttributeUserDataPlugIn.cpp for the implementation of this class
//

class CSampleAttributeUserDataPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleAttributeUserDataPlugIn();
  ~CSampleAttributeUserDataPlugIn();

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

CSampleAttributeUserDataPlugIn& SampleAttributeUserDataPlugIn();



