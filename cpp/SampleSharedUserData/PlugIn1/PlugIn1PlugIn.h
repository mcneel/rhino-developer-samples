/////////////////////////////////////////////////////////////////////////////
// PlugIn1PlugIn.h : main header file for the PlugIn1 plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPlugIn1PlugIn
// See PlugIn1PlugIn.cpp for the implementation of this class
//

class CPlugIn1PlugIn : public CRhinoUtilityPlugIn
{
public:
  CPlugIn1PlugIn();
  ~CPlugIn1PlugIn();

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

CPlugIn1PlugIn& PlugIn1PlugIn();



