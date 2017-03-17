/////////////////////////////////////////////////////////////////////////////
// SampleLocalizationPlugIn.h : main header file for the SampleLocalization plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleLocalizationPlugIn
// See SampleLocalizationPlugIn.cpp for the implementation of this class
//

class CSampleLocalizationPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleLocalizationPlugIn();
  ~CSampleLocalizationPlugIn();

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

CSampleLocalizationPlugIn& SampleLocalizationPlugIn();



