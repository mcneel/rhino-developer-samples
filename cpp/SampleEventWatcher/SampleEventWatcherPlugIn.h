/////////////////////////////////////////////////////////////////////////////
// SampleEventWatcherPlugIn.h : main header file for the SampleEventWatcher plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleEventWatcherPlugIn
// See SampleEventWatcherPlugIn.cpp for the implementation of this class
//

class CSampleEventWatcherPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleEventWatcherPlugIn();
  ~CSampleEventWatcherPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  CRhinoCommand::result MoveObjects(const CRhinoCommandContext& context);

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleEventWatcherPlugIn& SampleEventWatcherPlugIn();



