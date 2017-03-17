/////////////////////////////////////////////////////////////////////////////
// TestTextureMappingPlugIn.h : main header file for the TestTextureMapping plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CTestTextureMappingPlugIn
// See TestTextureMappingPlugIn.cpp for the implementation of this class
//

class CTestTextureMappingPlugIn : public CRhinoUtilityPlugIn
{
public:
  CTestTextureMappingPlugIn();
  ~CTestTextureMappingPlugIn();

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

CTestTextureMappingPlugIn& TestTextureMappingPlugIn();



