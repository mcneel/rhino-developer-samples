/////////////////////////////////////////////////////////////////////////////
// SampleAnalysisModePlugIn.h : main header file for the SampleAnalysisMode plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleAnalysisModePlugIn
// See SampleAnalysisModePlugIn.cpp for the implementation of this class
//

class CSampleAnalysisModePlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleAnalysisModePlugIn();
  ~CSampleAnalysisModePlugIn();

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

CSampleAnalysisModePlugIn& SampleAnalysisModePlugIn();



