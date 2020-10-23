
#pragma once

class CSampleRdkContentTaskRdkPlugIn;

// CSampleRdkContentTaskPlugIn
// See SampleRdkContentTaskPlugIn.cpp for the implementation of this class
//

class CSampleRdkContentTaskPlugIn : public CRhinoRenderPlugIn
{
public:
  // CSampleRdkContentTaskPlugIn constructor. The constructor is called when the
  // plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  // is loaded, CSampleRdkContentTaskPlugIn::OnLoadPlugIn() is called. The
  // constructor should be simple and solid. Do anything that might fail in
  // CSampleRdkContentTaskPlugIn::OnLoadPlugIn().
  CSampleRdkContentTaskPlugIn();
  
  // CSampleRdkContentTaskPlugIn destructor. The destructor is called to destroy
  // "thePlugIn" when the plug-in is unloaded. Immediately before the
  // DLL is unloaded, CSampleRdkContentTaskPlugIn::OnUnloadPlugin() is called. Do
  // not do too much here. Be sure to clean up any memory you have allocated
  // with onmalloc(), onrealloc(), oncalloc(), or onstrdup().
  virtual ~CSampleRdkContentTaskPlugIn() = default;

  // Required overrides
  
  // Plug-in name display string. This name is displayed by Rhino when
  // loading the plug-in, in the plug-in help menu, and in the Rhino
  // interface for managing plug-ins. 
  virtual const wchar_t* PlugInName() const override;
  
  // Plug-in version display string. This name is displayed by Rhino
  // when loading the plug-in and in the Rhino interface for 
  // managing plug-ins.
  virtual const wchar_t* PlugInVersion() const override;
  
  // Plug-in unique identifier. The identifier is used by Rhino for
  // managing plug-ins.
  virtual GUID PlugInID() const override;
  
  // Additional overrides

  // Called after the plug-in is loaded and the constructor has been
  // run. This is a good place to perform any significant initialization,
  // license checking, and so on.  This function must return TRUE for
  // the plug-in to continue to load.  
  virtual BOOL OnLoadPlugIn() override;

  // Called one time when plug-in is about to be unloaded. By this time,
  // Rhino's mainframe window has been destroyed, and some of the SDK
  // managers have been deleted. There is also no active document or active
  // view at this time. Thus, you should only be manipulating your own objects.
  // or tools here.  
  virtual void OnUnloadPlugIn() override;

  // Render overrides
  virtual CRhinoCommand::result Render(const CRhinoCommandContext& context, bool bPreview) override;
  virtual CRhinoCommand::result RenderWindow(
    const CRhinoCommandContext& context,
    bool render_preview,
    CRhinoView* view,
    const LPRECT rect,
    bool bInWindow,
    bool bBlowUp
  ) override;

  virtual plugin_load_time PlugInLoadTime() override { return load_plugin_at_startup; }

  BOOL SaveRenderedImage(ON_wString filename) override;
  BOOL CloseRenderWindow() override;

  // Render methods
  CRhinoCommand::result RenderQuiet( const CRhinoCommandContext& context, bool bPreview);
  UINT MainFrameResourceID() const;

private:
  ON_wString m_plugin_version;
  CSampleRdkContentTaskRdkPlugIn* m_pRdkPlugIn;

  // TODO: Add additional class information here
};

// Return a reference to the one and only CSampleRdkContentTaskPlugIn object
CSampleRdkContentTaskPlugIn& SampleRdkContentTaskPlugIn();
