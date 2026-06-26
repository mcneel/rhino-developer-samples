// SampleCppEtoInteropPlugIn.h : main header file for the SampleCppEtoInterop plug-in.
//

#pragma once

// CSampleCppEtoInteropPlugIn
// See SampleCppEtoInteropPlugIn.cpp for the implementation of this class
//

class CSampleCppEtoInteropPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleCppEtoInteropPlugIn();
  ~CSampleCppEtoInteropPlugIn() = default;

  // Required overrides
  const wchar_t* PlugInName() const override;
  const wchar_t* PlugInVersion() const override;
  GUID PlugInID() const override;

  // Additional overrides
  BOOL OnLoadPlugIn() override;
  void OnUnloadPlugIn() override;

private:
  ON_wString m_plugin_version;
};

// Return a reference to the one and only CSampleCppEtoInteropPlugIn object
CSampleCppEtoInteropPlugIn& SampleCppEtoInteropPlugIn();
