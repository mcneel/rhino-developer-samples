/////////////////////////////////////////////////////////////////////////////
// SampleObjectManagerPlugIn.h : main header file for the SampleObjectManager plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleObjectManagerPlugIn
// See SampleObjectManagerPlugIn.cpp for the implementation of this class
//

class CSampleObjectManagerPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleObjectManagerPlugIn();
  ~CSampleObjectManagerPlugIn();

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

CSampleObjectManagerPlugIn& SampleObjectManagerPlugIn();



