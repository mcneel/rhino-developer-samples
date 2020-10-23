
#include "stdafx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleRdkContentTaskPlugIn.h"
#include "SampleRdkContentTaskRdkPlugIn.h"
#include "resource.h"

// The plug-in object must be constructed before any plug-in classes derived
// from CRhinoCommand. The #pragma init_seg(lib) ensures that this happens.
#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME(L"SampleRdkContentTask");

// Rhino plug-in id
// Provide a unique uuid for this plug-in.
RHINO_PLUG_IN_ID(L"1B177077-8A07-45EA-912F-2BF10AB765A2");

// Rhino plug-in version
// Provide a version number string for this plug-in.
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in.
RHINO_PLUG_IN_DESCRIPTION(L"SampleRdkContentTask plug-in for Rhinoceros®");

// Rhino plug-in icon resource id
// Provide an icon resource this plug-in.
// Icon resource should contain 16, 24, 32, 48, and 256-pixel image sizes.
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON);

// Rhino plug-in developer declarations
// TODO: fill in the following developer declarations with
// your company information. Note, all of these declarations
// must be present or your plug-in will not load.
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleRdkContentTaskPlugIn object
static class CSampleRdkContentTaskPlugIn thePlugIn;

CSampleRdkContentTaskPlugIn& SampleRdkContentTaskPlugIn()
{
  // Return a reference to the one and only CSampleRdkContentTaskPlugIn object
  return thePlugIn;
}

CSampleRdkContentTaskPlugIn::CSampleRdkContentTaskPlugIn()
{
  // Description:
  //   CSampleRdkContentTaskPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleRdkContentTaskPlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleRdkContentTaskPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();

  m_pRdkPlugIn = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleRdkContentTaskPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleRdkContentTaskPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleRdkContentTaskPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {E620ED01-3284-47F1-BB10-B15CF062F1E4}
  return ON_UuidFromString(RhinoPlugInId());
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

BOOL CSampleRdkContentTaskPlugIn::OnLoadPlugIn()
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


  // If this assert fires, it's likely that the RDK has not yet been loaded by Rhino.
  // This can happen if you load your plug-in first, before the debug RDK and for some
  // reason it actually manages to find rdk.rhp on the search path. If this happens,
  // load protect your plug-in, restart Rhino and and load rdk.rhp using drag and drop
  // or the plug-in manager. Then un-load protect your plug-in.
  ASSERT(RhRdkIsAvailable());

  // TODO: Add render plug-in initialization code here.

  m_pRdkPlugIn = new CSampleRdkContentTaskRdkPlugIn;
  ON_wString str;
  if (!m_pRdkPlugIn->Initialize())
  {
    delete m_pRdkPlugIn;
    m_pRdkPlugIn = NULL;
    str.Format(L"Failed to load %s, version %s. RDK initialization failed\n", PlugInName(), PlugInVersion());
    RhinoApp().Print(str);
    return FALSE;
  }

  str.Format(L"Loading %s, version %s\n", PlugInName(), PlugInVersion());
  RhinoApp().Print(str);

  // TODO: Add render plug-in initialization code here.

  str.Format(L"Loading %s, version %s\n", PlugInName(), PlugInVersion());
  RhinoApp().Print(str);

  return TRUE;
}

void CSampleRdkContentTaskPlugIn::OnUnloadPlugIn()
{
  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.

  // TODO: Add render plug-in clean-up code here.

  if (NULL != m_pRdkPlugIn)
  {
    m_pRdkPlugIn->Uninitialize();
    delete m_pRdkPlugIn;
    m_pRdkPlugIn = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Render overrides

CRhinoCommand::result CSampleRdkContentTaskPlugIn::Render(const CRhinoCommandContext& context, bool bPreview)
{
	return CRhinoCommand::failure; 
}

CRhinoCommand::result CSampleRdkContentTaskPlugIn::RenderWindow(const CRhinoCommandContext& context, bool render_preview,
                                                                    CRhinoView* view, const LPRECT rect, bool bInWindow, bool bBlowUp)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(render_preview);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(rect);
	UNREFERENCED_PARAMETER(bInWindow);
	UNREFERENCED_PARAMETER(bBlowUp);

	return CRhinoCommand::failure;
}

CRhinoCommand::result CSampleRdkContentTaskPlugIn::RenderQuiet(const CRhinoCommandContext& context, bool bPreview)
{
  UNREFERENCED_PARAMETER(context);
  UNREFERENCED_PARAMETER(bPreview);

  return CRhinoCommand::failure;
}

BOOL CSampleRdkContentTaskPlugIn::SaveRenderedImage(ON_wString filename)
{
  // Description:
  //   Message sent from a script to save the rendering to a file.
  // Parameters:
  //   filename [in] The name of file to save.

  // TODO: Add file saving code here.
  return FALSE;
}

BOOL CSampleRdkContentTaskPlugIn::CloseRenderWindow()
{
  // Description:
  //   Close render window notification. Called when rendering is done and render window
  //   is no longer modal. (When RenderCommand returns if you leave the window up)

  return FALSE;
}

// Render methods

UINT CSampleRdkContentTaskPlugIn::MainFrameResourceID() const
{
  return IDR_RENDER;
}
