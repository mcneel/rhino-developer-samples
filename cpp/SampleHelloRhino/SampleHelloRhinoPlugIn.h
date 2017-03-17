/////////////////////////////////////////////////////////////////////////////
// SampleHelloRhinoPlugIn.h : main header file for the SampleHelloRhino plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleHelloRhinoPlugIn
// See SampleHelloRhinoPlugIn.cpp for the implementation of this class
//

class CSampleHelloRhinoPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleHelloRhinoPlugIn();
  ~CSampleHelloRhinoPlugIn();

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

CSampleHelloRhinoPlugIn& SampleHelloRhinoPlugIn();



