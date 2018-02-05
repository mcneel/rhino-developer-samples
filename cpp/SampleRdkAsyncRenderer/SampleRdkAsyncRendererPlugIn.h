
#pragma once

#include "SampleRdkAsyncRendererEventWatcher.h"

class CSampleRdkAsyncRendererRdkPlugIn;

class CSampleRdkAsyncRendererPlugIn : public CRhinoRenderPlugIn
{
public:
	// CSampleRdkAsyncRendererPlugIn constructor. The constructor is called when the
	// plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	// is loaded, CSampleRdkAsyncRendererPlugIn::OnLoadPlugIn() is called. The
	// constructor should be simple and solid. Do anything that might fail in
	// CSampleRdkAsyncRendererPlugIn::OnLoadPlugIn().
	CSampleRdkAsyncRendererPlugIn();

	// CSampleRdkAsyncRendererPlugIn destructor. The destructor is called to destroy
	// "thePlugIn" when the plug-in is unloaded. Immediately before the
	// DLL is unloaded, CSampleRdkAsyncRendererPlugIn::OnUnloadPlugin() is called. Do
	// not do too much here. Be sure to clean up any memory you have allocated
	// with onmalloc(), onrealloc(), oncalloc(), or onstrdup().
	virtual ~CSampleRdkAsyncRendererPlugIn() = default;

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

	// Render overrides
	CRhinoCommand::result Render(const CRhinoCommandContext& context, bool bPreview) override;
	CRhinoCommand::result RenderWindow(
		const CRhinoCommandContext& context,
		bool render_preview,
		CRhinoView* view,
		const LPRECT rect,
		bool bInWindow,
		bool bBlowUp
	) override;

	BOOL SaveRenderedImage(ON_wString filename) override;
	BOOL CloseRenderWindow() override;

	// Render methods
	CRhinoCommand::result RenderQuiet(const CRhinoCommandContext& context, bool bPreview);
	BOOL SceneChanged() const;
	void SetSceneChanged(BOOL bChanged);
	BOOL LightingChanged() const;
	void SetLightingChanged(BOOL bChanged);
	UINT MainFrameResourceID() const;

private:
	ON_wString m_plugin_version;
	CSampleRdkAsyncRendererEventWatcher m_event_watcher;
	class CSampleRdkAsyncRendererRdkPlugIn* m_pRdkPlugIn;

	// TODO: Add additional class information here
};

// Return a reference to the one and only CSampleRdkAsyncRendererPlugIn object
CSampleRdkAsyncRendererPlugIn& SampleRdkAsyncRendererPlugIn(void);
