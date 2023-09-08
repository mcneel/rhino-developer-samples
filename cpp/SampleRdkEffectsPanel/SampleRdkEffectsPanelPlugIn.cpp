
#include "stdafx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleRdkEffectsPanelPlugIn.h"
#include "SampleRdkEffectsPanelRdkPlugIn.h"
#include "SampleRdkEffectsPanelSdkRender.h"
#include "Resource.h"

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
RHINO_PLUG_IN_NAME(L"SampleRdkEffectsPanel");

// Rhino plug-in id
// Provide a unique uuid for this plug-in.
RHINO_PLUG_IN_ID(L"DD697365-D43D-4CEC-83EF-DC6AE9CB5A35");

// Rhino plug-in version
// Provide a version number string for this plug-in.
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in.
RHINO_PLUG_IN_DESCRIPTION(L"SampleRdkEffectsPanel plug-in for Rhinoceros®");

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

// The one and only CSampleRdkEffectsPanelPlugIn object
static class CSampleRdkEffectsPanelPlugIn thePlugIn;

CSampleRdkEffectsPanelPlugIn& SampleRdkEffectsPanelPlugIn(void)
{
	// Return a reference to the one and only CSampleRdkEffectsPanelPlugIn object
	return thePlugIn;
}

CSampleRdkEffectsPanelPlugIn::CSampleRdkEffectsPanelPlugIn()
{
	// Description:
	//   CSampleRdkEffectsPanelPlugIn constructor. The constructor is called when the
	//   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	//   is loaded, CSampleRdkEffectsPanelPlugIn::OnLoadPlugIn() is called. The
	//   constructor should be simple and solid. Do anything that might fail in
	//   CSampleRdkEffectsPanelPlugIn::OnLoadPlugIn().

	// TODO: Add construction code here
	m_plugin_version = RhinoPlugInVersion();

	m_pRdkPlugIn = NULL;
}

const wchar_t* CSampleRdkEffectsPanelPlugIn::PlugInName() const
{
	// Description:
	//   Plug-in name display string.  This name is displayed by Rhino when
	//   loading the plug-in, in the plug-in help menu, and in the Rhino
	//   interface for managing plug-ins.

	// TODO: Return a short, friendly name for the plug-in.
	return RhinoPlugInName();
}

const wchar_t* CSampleRdkEffectsPanelPlugIn::PlugInVersion() const
{
	// Description:
	//   Plug-in version display string. This name is displayed by Rhino
	//   when loading the plug-in and in the Rhino interface for managing
	//   plug-ins.

	// TODO: Return the version number of the plug-in.
	return m_plugin_version;
}

GUID CSampleRdkEffectsPanelPlugIn::PlugInID() const
{
	// Description:
	//   Plug-in unique identifier. The identifier is used by Rhino to
	//   manage the plug-ins.

	// TODO: Return a unique identifier for the plug-in.
	return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleRdkEffectsPanelPlugIn::OnLoadPlugIn()
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

	m_pRdkPlugIn = new CSampleRdkEffectsPanelRdkPlugIn;
	ON_wString str;
	if (!m_pRdkPlugIn->Initialize())
	{
		delete m_pRdkPlugIn;
		m_pRdkPlugIn = NULL;
		str.Format(L"Failed to load %ls, version %ls. RDK initialization failed\n", PlugInName(), PlugInVersion());
		RhinoApp().Print(str);
		return FALSE;
	}

	str.Format(L"Loading %ls, version %ls\n", PlugInName(), PlugInVersion());
	RhinoApp().Print(str);

	// TODO: Add render plug-in initialization code here.

	str.Format(L"Loading %ls, version %ls\n", PlugInName(), PlugInVersion());
	RhinoApp().Print(str);

	return TRUE;
}

void CSampleRdkEffectsPanelPlugIn::OnUnloadPlugIn()
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

CRhinoCommand::result CSampleRdkEffectsPanelPlugIn::Render(const CRhinoCommandContext& context, bool bPreview)
{
	// Description:
	//   Called by the Render and RenderPreview commands if this application is both
	//   a Render plug-in and is set as the default render engine.
	// Parameters:
	//   context [in] Command paramaters passed to the render command.
	//   bPreview [in] If true, a faster, lower quality rendering is expected.

	const auto* pDoc = context.Document();
	if (nullptr == pDoc)
		return CRhinoCommand::failure; 

	CSampleRdkEffectsPanelSdkRender render(context, *this, L"SampleRdkEffectsPanel", IDI_RENDER, bPreview);
	const auto size = render.RenderSize(*pDoc, true);
	if (CRhinoSdkRender::render_ok != render.Render(size))
		return CRhinoCommand::failure;

	return CRhinoCommand::success;
}

CRhinoCommand::result CSampleRdkEffectsPanelPlugIn::RenderWindow(
	const CRhinoCommandContext& context,
	bool render_preview,
	CRhinoView* view,
	const LPRECT rect,
	bool bInWindow,
	bool bBlowUp
)
{
	UNREFERENCED_PARAMETER(bBlowUp);

	// Description:
	//   Called by Render command if this application is both
	//   a Render plug-in and is set as the default render engine.
	// Parameters:
	//   context [in] Command paramaters passed to the render command.
	//   bPreview [in] If true, a faster, lower quality rendering is expected.
	//   view [in] View to render.
	//   rect [in] Rectangle to render in viewport window coordinates.

	CSampleRdkEffectsPanelSdkRender render(context, *this, L"SampleRdkEffectsPanel", IDI_RENDER, render_preview);
	if (CRhinoSdkRender::render_ok == render.RenderWindow(view, rect, bInWindow))
		return CRhinoCommand::success;

	return CRhinoCommand::failure;
}

//CRhinoCommand::result CSampleRdkEffectsPanelPlugIn::RenderQuiet(const CRhinoCommandContext& context, bool bPreview)
//{
//	UNREFERENCED_PARAMETER(context);
//	UNREFERENCED_PARAMETER(bPreview);
//
//	return CRhinoCommand::failure;
//}

BOOL CSampleRdkEffectsPanelPlugIn::SaveRenderedImage(ON_wString filename)
{
	// Description:
	//   Message sent from a script to save the rendering to a file.
	// Parameters:
	//   filename [in] The name of file to save.

	UNREFERENCED_PARAMETER(filename);

	// TODO: Add file saving code here.
	return FALSE;
}

BOOL CSampleRdkEffectsPanelPlugIn::CloseRenderWindow()
{
	// Description:
	//   Close render window notification. Called when rendering is done and render window
	//   is no longer modal. (When RenderCommand returns if you leave the window up)

	return FALSE;
}

// Render methods
//
//UINT CSampleRdkEffectsPanelPlugIn::MainFrameResourceID() const
//{
//	return IDR_RENDER;
//}
//
//BOOL CSampleRdkEffectsPanelPlugIn::SceneChanged() const
//{
//	return false;
//}
//
//void CSampleRdkEffectsPanelPlugIn::SetSceneChanged(BOOL bChanged)
//{
//}
//
//BOOL CSampleRdkEffectsPanelPlugIn::LightingChanged() const
//{
//	return false;
//}
//
//void CSampleRdkEffectsPanelPlugIn::SetLightingChanged(BOOL bChanged)
//{
//}
//
//