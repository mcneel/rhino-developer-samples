/////////////////////////////////////////////////////////////////////////////
// SampleDockbarPlugIn.h
//

#pragma once

// CSampleDockbarPlugIn

class CSampleDockbarPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleDockbarPlugIn();
  ~CSampleDockbarPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

private:
  ON_wString m_plugin_version;
};

CSampleDockbarPlugIn& SampleDockbarPlugIn();



