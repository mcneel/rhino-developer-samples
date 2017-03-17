/////////////////////////////////////////////////////////////////////////////
// SampleTabbedDockBarPlugIn.h : main header file for the SampleTabbedDockBar plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleTabbedDockBarPlugIn
// See SampleTabbedDockBarPlugIn.cpp for the implementation of this class
//

class CSampleTabbedDockBarPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleTabbedDockBarPlugIn();
  ~CSampleTabbedDockBarPlugIn();

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

CSampleTabbedDockBarPlugIn& SampleTabbedDockBarPlugIn();



