/////////////////////////////////////////////////////////////////////////////
// SampleThumbnailPreviewPlugIn.h : main header file for the SampleThumbnailPreview plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleThumbnailPreviewPlugIn
// See SampleThumbnailPreviewPlugIn.cpp for the implementation of this class
//

class CSampleThumbnailPreviewPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleThumbnailPreviewPlugIn();
  ~CSampleThumbnailPreviewPlugIn();

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

CSampleThumbnailPreviewPlugIn& SampleThumbnailPreviewPlugIn();



