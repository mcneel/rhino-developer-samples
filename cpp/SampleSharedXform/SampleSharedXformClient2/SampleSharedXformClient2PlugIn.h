#pragma once

// CSampleSharedXformClient2PlugIn
// See SampleSharedXformClient2PlugIn.cpp for the implementation of this class
//

class CSampleSharedXformClient2PlugIn : public CRhinoUtilityPlugIn
{
public:
  // CSampleSharedXformClient2PlugIn constructor. The constructor is called when the
  // plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  // is loaded, CSampleSharedXformClient2PlugIn::OnLoadPlugIn() is called. The
  // constructor should be simple and solid. Do anything that might fail in
  // CSampleSharedXformClient2PlugIn::OnLoadPlugIn().
  CSampleSharedXformClient2PlugIn();
  
  // CSampleSharedXformClient2PlugIn destructor. The destructor is called to destroy
  // "thePlugIn" when the plug-in is unloaded. Immediately before the
  // DLL is unloaded, CSampleSharedXformClient2PlugIn::OnUnloadPlugin() is called. Do
  // not do too much here. Be sure to clean up any memory you have allocated
  // with onmalloc(), onrealloc(), oncalloc(), or onstrdup().
  ~CSampleSharedXformClient2PlugIn() = default;

  // Required overrides
  
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

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

// Return a reference to the one and only CSampleSharedXformClient2PlugIn object
CSampleSharedXformClient2PlugIn& SampleSharedXformClient2PlugIn();



