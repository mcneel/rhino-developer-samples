/////////////////////////////////////////////////////////////////////////////
// SampleRhinoPlugIn.h : main header file for the SampleRhino plug-in
//

#pragma once
#include "SampleRhinoObject.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleRhinoPlugIn
// See SampleRhinoPlugIn.cpp for the implementation of this class
//

class CSampleRhinoPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleRhinoPlugIn();
  ~CSampleRhinoPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Additional overrides
  CRhinoPlugIn::plugin_load_time PlugInLoadTime();
  LPUNKNOWN GetPlugInObjectInterface( const ON_UUID& iid );

private:
  ON_wString m_plugin_version;
	CSampleRhinoObject m_script_object;
};

CSampleRhinoPlugIn& SampleRhinoPlugIn();



