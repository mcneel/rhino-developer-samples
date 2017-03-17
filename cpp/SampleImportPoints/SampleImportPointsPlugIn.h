#pragma once

class CSampleImportPointsPlugIn : public CRhinoFileImportPlugIn
{
public:
  CSampleImportPointsPlugIn();
  ~CSampleImportPointsPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  // File import overrides
  void AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options);
  BOOL ReadFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileReadOptions& options);

private:
  BOOL ParsePointValue(const wchar_t* string, const wchar_t* delimiter, ON_3dPoint& pt);
  BOOL ParseRealValue(const wchar_t* string, double& value);

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CSampleImportPointsPlugIn& SampleImportPointsPlugIn();



