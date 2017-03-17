/////////////////////////////////////////////////////////////////////////////
// SampleCommandsPlugIn.h : main header file for the SampleCommands plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleCommandsPlugIn
// See SampleCommandsPlugIn.cpp for the implementation of this class
//

class CSampleCommandsPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleCommandsPlugIn();
  ~CSampleCommandsPlugIn();

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

CSampleCommandsPlugIn& SampleCommandsPlugIn();



