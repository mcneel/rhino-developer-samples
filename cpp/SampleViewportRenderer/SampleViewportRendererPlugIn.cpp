// SampleViewportRendererPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInDeclare.h"
#include "SampleViewportRendererPlugIn.h"
#include "SampleRenderer.h"
#include "SampleDisplayPipeline.h"

// The plug-in object must be constructed before any plug-in classes derived
// from CRhinoCommand. The #pragma init_seg(lib) ensures that this happens.
#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SampleViewportRenderer" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"336403E8-9B17-41C5-910F-7D951D99E607" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__ "  " __TIME__ )

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/Rhino6Samples_CPP");

// The one and only CSampleViewportRendererPlugIn object
static CSampleViewportRendererPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleViewportRendererPlugIn definition

CSampleViewportRendererPlugIn& SampleViewportRendererPlugIn()
{
  // Return a reference to the one and only CSampleViewportRendererPlugIn object
  return thePlugIn;
}

CSampleViewportRendererPlugIn::CSampleViewportRendererPlugIn()
  : m_pRenderer(0)
{
  // Description:
  //   CSampleViewportRendererPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleViewportRendererPlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleViewportRendererPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();

  // Reset everything to idle state
  m_bRedrawRequested = false;
  m_uTimerProcId = 0;

  // Create a dib to render to.
  // We want to copy this image into Rhino's viewport DC
  // and therefore we need to set select_into_mem_dc to true.
  m_renderImageDib.CreateDib(100, 100, 24, true);
  m_renderImageDib.Clear(RGB(0, 0, 0));
}

CSampleViewportRendererPlugIn::~CSampleViewportRendererPlugIn()
{
  // Description:
  //   CSampleViewportRendererPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleViewportRendererPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleViewportRendererPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleViewportRendererPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleViewportRendererPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {336403E8-9B17-41C5-910F-7D951D99E607}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleViewportRendererPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // Remarks:
  //    Plug-ins are not loaded until after Rhino is started and a default document
  //    is created.  Because the default document already exists
  //    CRhinoEventWatcher::On????Document() functions are not called for the default
  //    document.  If you need to do any document initialization/synchronization then
  //    override this function and do it here.  It is not necessary to call
  //    CPlugIn::OnLoadPlugIn() from your derived class.

  // TODO: Add plug-in initialization code here.

  // Look if the display mode is already added...
  DisplayAttrsMgrListDesc* pDisplayAttrsMgrListDesc = CRhinoDisplayAttrsMgr::FindDisplayAttrsDesc(DisplayModeID());

  // ... and if it isn't then add it.
  if (0 == pDisplayAttrsMgrListDesc)
  {
    pDisplayAttrsMgrListDesc = CRhinoDisplayAttrsMgr::AppendNewEntry();
    if (0 != pDisplayAttrsMgrListDesc && 0 != pDisplayAttrsMgrListDesc->m_pAttrs)
    {
      // Show it in the drop down menu
      pDisplayAttrsMgrListDesc->m_bAddToMenu = true;

      // Set it's guid, name and pipeline class
      pDisplayAttrsMgrListDesc->m_pAttrs->SetUuid(DisplayModeID());
      pDisplayAttrsMgrListDesc->m_pAttrs->SetName(PlugInName());
      pDisplayAttrsMgrListDesc->m_pAttrs->SetPipeline(RUNTIME_CLASS(CSampleDisplayPipeline));
    }
  }
  else
  {
    pDisplayAttrsMgrListDesc->m_pAttrs->SetName(PlugInName());
    pDisplayAttrsMgrListDesc->m_pAttrs->SetPipeline(RUNTIME_CLASS(CSampleDisplayPipeline));
  }

  // Start the renderer
  StartRenderer();

  // Enable our displa mode event watcher
  m_display_mode_watcher.Register();
  m_display_mode_watcher.Enable(true);

  return TRUE;
}

void CSampleViewportRendererPlugIn::OnUnloadPlugIn()
{
  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.

  // TODO: Add plug-in cleanup code here.

  StopRenderer();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSampleViewportRendererPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return FALSE;
}

BOOL CSampleViewportRendererPlugIn::OnDisplayPlugInHelp( HWND hWnd ) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp( hWnd );
}

/////////////////////////////////////////////////////////////////////////////
// Load time override

CRhinoPlugIn::plugin_load_time CSampleViewportRendererPlugIn::PlugInLoadTime()
{
  // This plugin has to be loaded at startup in order to register the pipeline.
  return CRhinoPlugIn::load_plugin_at_startup;
}

/////////////////////////////////////////////////////////////////////////////
// Custom renderer members

GUID CSampleViewportRendererPlugIn::DisplayModeID() const
{
  // Guid for our custom display mode
  // {9913CEEE-8E5C-4F69-909D-79B73F54EAFE}
  static const GUID SampleViewportRendererDisplayAttrs_UUID =
  { 0x9913CEEE, 0x8E5C, 0x4F69,{ 0x90, 0x9D, 0x79, 0xB7, 0x3F, 0x54, 0xEA, 0xFE } };
  return SampleViewportRendererDisplayAttrs_UUID;
}

// Starts the renderer
void CSampleViewportRendererPlugIn::StartRenderer()
{
  if (0 == m_pRenderer)
  {
    // Create and start the renderer
    m_pRenderer = new CSampleRenderer();
    if (0 != m_pRenderer)
      m_pRenderer->StartRenderProcess();

    // Set timer for the viewport updating
    CWnd* pMainWnd = RhinoApp().GetMainWnd();
    if (0 != pMainWnd)
      m_uTimerProcId = pMainWnd->SetTimer(CSampleViewportRendererPlugIn::s_uTimerProcEvent, 100, CSampleViewportRendererPlugIn::RedrawTimerProc);
  }
}

// Stops the renderer
void CSampleViewportRendererPlugIn::StopRenderer()
{
  if (0 != m_pRenderer)
  {
    // Kill the timer
    CWnd* pMainWnd = RhinoApp().GetMainWnd();
    if (0 != pMainWnd)
      pMainWnd->KillTimer(m_uTimerProcId);
    m_uTimerProcId = 0;

    // Stop renderer and delete it
    m_pRenderer->StopRenderProcess();
    delete m_pRenderer;
    m_pRenderer = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Rendered image access

CRhinoDib* CSampleViewportRendererPlugIn::LockRenderImage()
{
  // Lock to prevent two threads from accessing the render image
  // simultaniously and return pointer to it.
  m_cs.Lock();
  return &m_renderImageDib;
}

void CSampleViewportRendererPlugIn::UnlockRenderImage(CRhinoDib* pRenderImage)
{
  UNREFERENCED_PARAMETER(pRenderImage);

  // Allow access to render image again.
  m_cs.Unlock();
}

/////////////////////////////////////////////////////////////////////////////
// Viewport updating

void CSampleViewportRendererPlugIn::RequestRedraw()
{
  // Set flag so that the timer proc in main thread knows to update the viewport
  m_bRedrawRequested = true;
}

void CSampleViewportRendererPlugIn::AcknowledgeRedraw()
{
  // Viewport is updated
  m_bRedrawRequested = false;
}

void CSampleViewportRendererPlugIn::OnRedrawTimer()
{
  // This method gets called from the timer procedure.
  // Redraw if the flag is set.
  if (m_bRedrawRequested)
  {
    CRhinoDoc* pDoc = RhinoApp().ActiveDoc();
    if (0 != pDoc)
      pDoc->DeferredRedraw();
  }
}

// Viewport update timer event
UINT CSampleViewportRendererPlugIn::s_uTimerProcEvent = 6060;

// Viewport update timer callback
void CALLBACK CSampleViewportRendererPlugIn::RedrawTimerProc(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
  UNREFERENCED_PARAMETER(hWnd);
  UNREFERENCED_PARAMETER(nMsg);
  UNREFERENCED_PARAMETER(nIDEvent);
  UNREFERENCED_PARAMETER(dwTime);

  // Call the OnRedrawTimer of our plugin instance
  SampleViewportRendererPlugIn().OnRedrawTimer();
}


/////////////////////////////////////////////////////////////////////////////
// CSampleViewportRendererDisplayModeChanged event watcher

CSampleViewportRendererDisplayModeChanged::CSampleViewportRendererDisplayModeChanged()
  : CRhinoDisplayModeChanged(SampleViewportRendererPlugIn().PlugInID())
{
}

// Rhino will call Notify() immediately after changing the view's display mode.
// WARNING: Never modify the Rhino document or application state in a Notify override.
// Never attempt to change the view's (or any other view) display mode inside
// your Notify routine, or you will eventually crash Rhino.
void CSampleViewportRendererDisplayModeChanged::Notify(const class CRhinoDisplayModeChanged::CParameters& params)
{
  // For more information, see rhinoSdkEventWatcher.h

  if ((0 != params.m_vp) && (params.m_changed_display_mode_id != params.m_old_display_mode_id))
  {
    // A view's display mode has been set.
  }
  else if ((params.m_vp != NULL) && (params.m_changed_display_mode_id == params.m_old_display_mode_id))
  {
    // The attributes of the display mode used by the active view have changed.
  }
  else if ((params.m_vp == NULL) && (params.m_old_display_mode_id == ON_nil_uuid))
  {
    // The attributes of a specific display mode have changed.
  }
}
