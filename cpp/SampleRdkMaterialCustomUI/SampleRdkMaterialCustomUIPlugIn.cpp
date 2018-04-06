
#include "stdafx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleRdkMaterialCustomUIPlugIn.h"
#include "SampleRdkMaterialCustomUIRdkPlugIn.h"
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
RHINO_PLUG_IN_NAME(L"SampleRdkMaterialCustomUI");

// Rhino plug-in id
// Provide a unique uuid for this plug-in.
RHINO_PLUG_IN_ID(L"05712030-8E1B-40C7-A40D-A63110B266B2");

// Rhino plug-in version
// Provide a version number string for this plug-in.
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in.
RHINO_PLUG_IN_DESCRIPTION(L"SampleRdkMaterialCustomUI plug-in for Rhinoceros®");

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

// The one and only CSampleRdkMaterialCustomUIPlugIn object
static class CSampleRdkMaterialCustomUIPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleRdkMaterialCustomUIPlugIn definition

CSampleRdkMaterialCustomUIPlugIn& SampleRdkMaterialCustomUIPlugIn()
{
	// Return a reference to the one and only CSampleRdkMaterialCustomUIPlugIn object
	return thePlugIn;
}

CSampleRdkMaterialCustomUIPlugIn::CSampleRdkMaterialCustomUIPlugIn()
{
	// Description:
	//   CSampleRdkMaterialCustomUIPlugIn constructor. The constructor is called when the
	//   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	//   is loaded, CSampleRdkMaterialCustomUIPlugIn::OnLoadPlugIn() is called. The
	//   constructor should be simple and solid. Do anything that might fail in
	//   CSampleRdkMaterialCustomUIPlugIn::OnLoadPlugIn().

	// TODO: Add construction code here
	m_plugin_version = RhinoPlugInVersion();

	m_pRdkPlugIn = NULL;
}

const wchar_t* CSampleRdkMaterialCustomUIPlugIn::PlugInName() const
{
	return RhinoPlugInName();
}

const wchar_t* CSampleRdkMaterialCustomUIPlugIn::PlugInVersion() const
{
	return m_plugin_version;
}

GUID CSampleRdkMaterialCustomUIPlugIn::PlugInID() const
{
	return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleRdkMaterialCustomUIPlugIn::OnLoadPlugIn()
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

	m_pRdkPlugIn = new CSampleRdkMaterialCustomUIRdkPlugIn;
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

	m_event_watcher.Register();
	m_event_watcher.Enable(TRUE);

	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	if (nullptr != doc)
	{
		m_event_watcher.OnNewDocument(*doc);
	}

	return TRUE;
}

void CSampleRdkMaterialCustomUIPlugIn::OnUnloadPlugIn()
{
	// Description:
	//    Called one time when plug-in is about to be unloaded. By this time,
	//    Rhino's mainframe window has been destroyed, and some of the SDK
	//    managers have been deleted. There is also no active document or active
	//    view at this time. Thus, you should only be manipulating your own objects.
	//    or tools here.

	// TODO: Add render plug-in clean-up code here.
	m_event_watcher.Enable(FALSE);
	m_event_watcher.UnRegister();

	if (nullptr != m_pRdkPlugIn)
	{
		m_pRdkPlugIn->Uninitialize();
		delete m_pRdkPlugIn;
		m_pRdkPlugIn = nullptr;
	}
}

CRhinoCommand::result CSampleRdkMaterialCustomUIPlugIn::Render(const CRhinoCommandContext& context, bool bPreview)
{
	// See project SampleRdkAsyncRenderer for an example.

	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(bPreview);

	return CRhinoCommand::failure;
}
