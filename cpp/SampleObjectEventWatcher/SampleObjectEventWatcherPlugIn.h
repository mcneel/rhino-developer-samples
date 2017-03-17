/////////////////////////////////////////////////////////////////////////////
// SampleObjectEventWatcherPlugIn.h : main header file for the SampleObjectEventWatcher plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleObjectEventWatcherPlugIn
// See SampleObjectEventWatcherPlugIn.cpp for the implementation of this class
//

class CSampleObjectEventWatcherPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleObjectEventWatcherPlugIn();
  ~CSampleObjectEventWatcherPlugIn();

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

CSampleObjectEventWatcherPlugIn& SampleObjectEventWatcherPlugIn();



