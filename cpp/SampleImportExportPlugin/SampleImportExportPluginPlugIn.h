#pragma once

// CSampleImportExportPluginPlugIn
//
// One plug-in that both reads and writes a file format. Before
// CRhinoImportExportPlugIn existed, a format supported in both directions needed
// two plug-ins: one deriving from CRhinoFileImportPlugIn and one from
// CRhinoFileExportPlugIn.
//
// CRhinoImportExportPlugIn is a sibling of those two, not a child of either.

class CSampleImportExportPluginPlugIn : public CRhinoImportExportPlugIn
{
public:
  CSampleImportExportPluginPlugIn();
  ~CSampleImportExportPluginPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const override;
  const wchar_t* PlugInVersion() const override;
  GUID PlugInID() const override;

  // Additional overrides
  BOOL OnLoadPlugIn() override;
  void OnUnloadPlugIn() override;

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const override;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const override;

  // The file types this plug-in reads. Called when Rhino builds the Open and
  // Import dialogs.
  void AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options) override;

  // The file types this plug-in writes. Called when Rhino builds the Save and
  // Export dialogs.
  void AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileWriteOptions& options) override;

  // index is into the list built by the CRhinoFileReadOptions overload of
  // AddFileType, which is indexed separately from the write list.
  BOOL ReadFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileReadOptions& options) override;

  // index is into the list built by the CRhinoFileWriteOptions overload of
  // AddFileType.
  int WriteFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileWriteOptions& options) override;

  // There is one of these, not one per direction, so the same override serves the
  // Open dialog and the Save dialog. CRhinoFileType::IsForImport() tells them
  // apart. The description string does not: this plug-in uses one description for
  // both directions, which is normal for a format that is read and written.
  void DisplayOptionsDialog(HWND parent, const CRhinoFileType& fileType) override;

private:
  ON_wString m_plugin_version;

  // Written into the file header and checked when reading.
  static const wchar_t* FileHeader();
};

// Return a reference to the one and only CSampleImportExportPluginPlugIn object
CSampleImportExportPluginPlugIn& SampleImportExportPluginPlugIn();
