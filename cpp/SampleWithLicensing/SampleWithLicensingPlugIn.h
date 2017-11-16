#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleWithLicensingPlugIn
// See SampleWithLicensingPlugIn.cpp for the implementation of this class
//

class CSampleWithLicensingPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleWithLicensingPlugIn();
  ~CSampleWithLicensingPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();
  plugin_load_time PlugInLoadTime();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleWithLicensingPlugIn& SampleWithLicensingPlugIn();



