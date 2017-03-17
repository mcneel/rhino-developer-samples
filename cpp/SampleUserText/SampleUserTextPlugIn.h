/////////////////////////////////////////////////////////////////////////////
// SampleUserTextPlugIn.h : main header file for the SampleUserText plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleUserTextPlugIn
// See SampleUserTextPlugIn.cpp for the implementation of this class
//

class CSampleUserTextPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleUserTextPlugIn();
  ~CSampleUserTextPlugIn();

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

CSampleUserTextPlugIn& SampleUserTextPlugIn();



