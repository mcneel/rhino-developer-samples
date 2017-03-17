/////////////////////////////////////////////////////////////////////////////
// PlugIn2PlugIn.h : main header file for the PlugIn2 plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPlugIn2PlugIn
// See PlugIn2PlugIn.cpp for the implementation of this class
//

class CPlugIn2PlugIn : public CRhinoUtilityPlugIn
{
public:
  CPlugIn2PlugIn();
  ~CPlugIn2PlugIn();

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

CPlugIn2PlugIn& PlugIn2PlugIn();



