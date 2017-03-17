/////////////////////////////////////////////////////////////////////////////
// SampleHistoryPlugIn.h : main header file for the SampleHistory plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleHistoryPlugIn
// See SampleHistoryPlugIn.cpp for the implementation of this class
//

class CSampleHistoryPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleHistoryPlugIn();
  ~CSampleHistoryPlugIn();

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

CSampleHistoryPlugIn& SampleHistoryPlugIn();



