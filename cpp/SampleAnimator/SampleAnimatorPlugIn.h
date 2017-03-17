/////////////////////////////////////////////////////////////////////////////
// SampleAnimatorPlugIn.h : main header file for the SampleAnimator plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleAnimatorPlugIn
// See SampleAnimatorPlugIn.cpp for the implementation of this class
//

class CSampleAnimatorPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleAnimatorPlugIn();
  ~CSampleAnimatorPlugIn();

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

CSampleAnimatorPlugIn& SampleAnimatorPlugIn();



