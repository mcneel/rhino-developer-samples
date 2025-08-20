#pragma once

// CSampleImportGeomviewPlugIn

class CSampleImportGeomviewPlugIn : public CRhinoFileImportPlugIn
{
public:
  // CSampleImportGeomviewPlugIn constructor. The constructor is called when the
  // plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  // is loaded, CSampleImportGeomviewPlugIn::OnLoadPlugIn() is called. The
  // constructor should be simple and solid. Do anything that might fail in
  // CSampleImportGeomviewPlugIn::OnLoadPlugIn().
  CSampleImportGeomviewPlugIn();
  
  // CSampleImportGeomviewPlugIn destructor. The destructor is called to destroy
  // "thePlugIn" when the plug-in is unloaded. Immediately before the
  // DLL is unloaded, CSampleImportGeomviewPlugIn::OnUnloadPlugin() is called. Do
  // not do too much here. Be sure to clean up any memory you have allocated
  // with onmalloc(), onrealloc(), oncalloc(), or onstrdup().
  ~CSampleImportGeomviewPlugIn();

  // Plug-in name display string. This name is displayed by Rhino when
  // loading the plug-in, in the plug-in help menu, and in the Rhino
  // interface for managing plug-ins. 
  const wchar_t* PlugInName() const override;

  // Plug-in version display string. This name is displayed by Rhino
  // when loading the plug-in and in the Rhino interface for 
  // managing plug-ins.
  const wchar_t* PlugInVersion() const override;

  // Plug-in unique identifier. The identifier is used by Rhino for
  // managing plug-ins.
  GUID PlugInID() const override;

  // Additional overrides

  // Called after the plug-in is loaded and the constructor has been
  // run. This is a good place to perform any significant initialization,
  // license checking, and so on.  This function must return TRUE for
  // the plug-in to continue to load.  
  BOOL OnLoadPlugIn() override;

  // Called one time when plug-in is about to be unloaded. By this time,
  // Rhino's mainframe window has been destroyed, and some of the SDK
  // managers have been deleted. There is also no active document or active
  // view at this time. Thus, you should only be manipulating your own objects.
  // or tools here.  
  void OnUnloadPlugIn() override;

  // Online help overrides

  BOOL AddToPlugInHelpMenu() const override;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const override;

  // File import overrides

  // Called by Rhino when displaying the open file dialog
  // Add supported file type extensions here.  
  void AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options);
  
  // Called by Rhino to read document geometry from an external file.
  BOOL ReadFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileReadOptions& options);

private:
  BOOL ReadOffFile(const wchar_t* filename, CRhinoDoc& doc);

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

// Return a reference to the one and only SampleImportGeomviewPlugIn object
CSampleImportGeomviewPlugIn& SampleImportGeomviewPlugIn();
