/////////////////////////////////////////////////////////////////////////////
// SampleDisplayConduitsPlugIn.h : main header file for the SampleDisplayConduits plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleDisplayConduitsPlugIn
// See SampleDisplayConduitsPlugIn.cpp for the implementation of this class
//

class CSampleDisplayConduitsPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleDisplayConduitsPlugIn();
  ~CSampleDisplayConduitsPlugIn();

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

CSampleDisplayConduitsPlugIn& SampleDisplayConduitsPlugIn();



