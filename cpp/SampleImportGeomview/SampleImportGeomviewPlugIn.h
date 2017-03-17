/////////////////////////////////////////////////////////////////////////////
// SampleImportGeomviewPlugIn.h : main header file for the SampleImportGeomview plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleImportGeomviewPlugIn
// See SampleImportGeomviewPlugIn.cpp for the implementation of this class
//

class CSampleImportGeomviewPlugIn : public CRhinoFileImportPlugIn
{
public:
  CSampleImportGeomviewPlugIn();
  ~CSampleImportGeomviewPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // File import overrides
  void AddFileType( ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options );
  BOOL ReadFile( const wchar_t* filename, int index, CRhinoDoc& dox, const CRhinoFileReadOptions& options );

private:
  BOOL ReadOffFile( const wchar_t* filename, CRhinoDoc& doc );

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleImportGeomviewPlugIn& SampleImportGeomviewPlugIn();



