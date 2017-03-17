/////////////////////////////////////////////////////////////////////////////
// SampleMarkerPlugIn.h : main header file for the SampleMarker plug-in
//

#pragma once

#include "SampleMarkerEventWatcher.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleMarkerPlugIn
// See SampleMarkerPlugIn.cpp for the implementation of this class
//

class CSampleMarkerPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleMarkerPlugIn();
  ~CSampleMarkerPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
  CSampleMarkerEventWatcher m_watcher;
};

CSampleMarkerPlugIn& SampleMarkerPlugIn();



