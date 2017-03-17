/////////////////////////////////////////////////////////////////////////////
// SampleScrollTabbedDockBarPlugIn.h : main header file for the SampleScrollTabbedDockBar plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleScrollTabbedDockBarPlugIn
// See SampleScrollTabbedDockBarPlugIn.cpp for the implementation of this class
//

class CSampleScrollTabbedDockBarPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleScrollTabbedDockBarPlugIn();
  ~CSampleScrollTabbedDockBarPlugIn();

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

CSampleScrollTabbedDockBarPlugIn& SampleScrollTabbedDockBarPlugIn();



