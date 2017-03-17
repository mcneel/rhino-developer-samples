/////////////////////////////////////////////////////////////////////////////
// SampleCustomGripsPlugIn.h : main header file for the SampleCustomGrips plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleCustomGripsPlugIn
// See SampleCustomGripsPlugIn.cpp for the implementation of this class
//

class CSampleCustomGripsPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleCustomGripsPlugIn();
  ~CSampleCustomGripsPlugIn();

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

CSampleCustomGripsPlugIn& SampleCustomGripsPlugIn();



