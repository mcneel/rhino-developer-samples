/////////////////////////////////////////////////////////////////////////////
// SampleExportMeshPlugIn.h : main header file for the SampleExportMesh plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleExportMeshPlugIn
// See SampleExportMeshPlugIn.cpp for the implementation of this class
//

class CSampleExportMeshPlugIn : public CRhinoFileExportPlugIn
{
public:
  CSampleExportMeshPlugIn();
  ~CSampleExportMeshPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // File export overrides
  void AddFileType( ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileWriteOptions& options );
  BOOL WriteFile( const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileWriteOptions& options );

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
  ON_MeshParameters m_mesh_parameters;
  int m_mesh_ui_style;
};

CSampleExportMeshPlugIn& SampleExportMeshPlugIn();



