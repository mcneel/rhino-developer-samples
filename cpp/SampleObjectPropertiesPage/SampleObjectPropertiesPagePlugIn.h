/////////////////////////////////////////////////////////////////////////////
// SampleObjectPropertiesPagePlugIn.h : main header file for the SampleObjectPropertiesPage plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleObjectPropertiesPagePlugIn
// See SampleObjectPropertiesPagePlugIn.cpp for the implementation of this class
//

class CSampleObjectPropertiesPagePlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleObjectPropertiesPagePlugIn();
  ~CSampleObjectPropertiesPagePlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Optional overrides
  void AddPagesToObjectPropertiesDialog( ON_SimpleArray<class CRhinoObjectPropertiesDialogPage*>& pages );

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleObjectPropertiesPagePlugIn& SampleObjectPropertiesPagePlugIn();



