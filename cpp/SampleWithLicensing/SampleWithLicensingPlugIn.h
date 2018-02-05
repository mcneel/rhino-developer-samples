#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleWithLicensingPlugIn
// See SampleWithLicensingPlugIn.cpp for the implementation of this class
//

class CSampleWithLicensingPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleWithLicensingPlugIn();
  ~CSampleWithLicensingPlugIn() = default;

  // Required overrides
  const wchar_t* PlugInName() const override;
  const wchar_t* PlugInVersion() const override;
  GUID PlugInID() const override;
  BOOL OnLoadPlugIn() override;
  void OnUnloadPlugIn() override;

private:
  ON_wString m_plugin_version;
  ON_wString m_text_mask;
  CRhinoPlugIn::license_capabilities m_capabilities;
};

CSampleWithLicensingPlugIn& SampleWithLicensingPlugIn();
