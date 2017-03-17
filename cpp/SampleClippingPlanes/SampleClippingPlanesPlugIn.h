/////////////////////////////////////////////////////////////////////////////
// SampleClippingPlanesPlugIn.h : main header file for the SampleClippingPlanes plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleClippingPlanesPlugIn
// See SampleClippingPlanesPlugIn.cpp for the implementation of this class
//

class CSampleClippingPlanesPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleClippingPlanesPlugIn();
  ~CSampleClippingPlanesPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  int StandardViewportIds( CRhinoDoc& doc, ON_SimpleArray<UUID>& viewport_ids );

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleClippingPlanesPlugIn& SampleClippingPlanesPlugIn();



