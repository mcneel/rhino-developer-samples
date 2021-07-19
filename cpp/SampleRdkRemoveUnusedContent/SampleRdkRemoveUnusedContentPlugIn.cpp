
#include "stdafx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleRdkRemoveUnusedContentPlugIn.h"
#include "SampleRdkRemoveUnusedContentRdkPlugIn.h"
#include "Resource.h"

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
RHINO_PLUG_IN_NAME( L"SampleRdkRemoveUnusedContent" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"E0C7669C-EE93-4260-A3D5-366BDA9ACAFB" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__ "  " __TIME__ )

// Rhino plug-in description
// Provide a description of this plug-in
RHINO_PLUG_IN_DESCRIPTION(L"SampleRdkRemoveUnusedContent plug-in for Rhinoceros®");

// Rhino plug-in icon resource id
// Provide an icon resource this plug-in.
// Icon resource should contain 16, 24, 32, 48, and 256-pixel image sizes.
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON1);

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleRdkRemoveUnusedContentPlugIn object
static CSampleRdkRemoveUnusedContentPlugIn thePlugIn;

CSampleRdkRemoveUnusedContentPlugIn& SampleRdkRemoveUnusedContentPlugIn()
{
  // Return a reference to the one and only CSampleRdkRemoveUnusedContentPlugIn object
  return thePlugIn;
}

CSampleRdkRemoveUnusedContentPlugIn::CSampleRdkRemoveUnusedContentPlugIn()
{
	// Description:
	//   CSampleRdkRemoveUnusedContentPlugIn constructor. The constructor is called when the
	//   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	//   is loaded, CSampleRdkRemoveUnusedContentPlugIn::OnLoadPlugIn() is called. The
	//   constructor should be simple and solid. Do anything that might fail in
	//   CSampleRdkRemoveUnusedContentPlugIn::OnLoadPlugIn().

	// TODO: Add construction code here
	m_plugin_version = RhinoPlugInVersion();
}

CSampleRdkRemoveUnusedContentPlugIn::~CSampleRdkRemoveUnusedContentPlugIn()
{
	// Description:
	//   CSampleRdkRemoveUnusedContentPlugIn destructor. The destructor is called to destroy
	//   "thePlugIn" when the plug-in is unloaded. Immediately before the
	//   DLL is unloaded, CSampleRdkRemoveUnusedContentPlugIn::OnUnloadPlugin() is called. Do
	//   not do too much here. Be sure to clean up any memory you have allocated
	//   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

	// TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleRdkRemoveUnusedContentPlugIn::PlugInName() const
{
	// Description:
	//   Plug-in name display string.  This name is displayed by Rhino when
	//   loading the plug-in, in the plug-in help menu, and in the Rhino
	//   interface for managing plug-ins.

	// TODO: Return a short, friendly name for the plug-in.
	return RhinoPlugInName();
}

const wchar_t* CSampleRdkRemoveUnusedContentPlugIn::PlugInVersion() const
{
	// Description:
	//   Plug-in version display string. This name is displayed by Rhino
	//   when loading the plug-in and in the Rhino interface for managing
	//   plug-ins.

	// TODO: Return the version number of the plug-in.
	return m_plugin_version;
}

GUID CSampleRdkRemoveUnusedContentPlugIn::PlugInID() const
{
	// Description:
	//   Plug-in unique identifier. The identifier is used by Rhino to
	//   manage the plug-ins.

	// TODO: Return a unique identifier for the plug-in.
	// {F4B670E4-C5CF-4FAE-A77A-E1BBE6764017}
	return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleRdkRemoveUnusedContentPlugIn::OnLoadPlugIn()
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

	// If this assert fires, it's likely that the RDK executable (rdk_d.rhp) is not loaded into Rhino4_d.exe
	// This can happen if you load your plug-in first, before the debug RDK and for some reason it actually
	// manages to find rdk_d.rhp on the search path.  If this happens, load protect your plug-in, restart Rhino4_d
	// and load rdk_d.rhp using drag and drop or the plug-in manager.  Then un-load protect your plug-in.  Restart
	// Rhino and all should be fine.
	// Note that this problem doesn't affect release versions because the release RDK installer causes the RDK to
	// autoload before all other plug-ins from the moment the installer completes.
	ASSERT(RhRdkIsAvailable());

	// TODO: Add render plug-in initialization code here.

	m_pRdkPlugIn = new CSampleRdkRemoveUnusedContentRdkPlugIn;

	ON_wString wStr;

	if (!m_pRdkPlugIn->Initialize())
	{
		delete m_pRdkPlugIn;
		m_pRdkPlugIn = NULL;

		wStr.Format(L"Failed to load %ls, version %ls.  RDK initialization failed\n", PlugInName(), PlugInVersion());
		RhinoApp().Print(wStr);

		return FALSE;
	}

	wStr.Format(L"Loading %ls, version %ls\n", PlugInName(), PlugInVersion());
	RhinoApp().Print(wStr);

#ifdef _DEBUG
	RhinoApp().Print(L"running on RDK version %ls\n", static_cast<const wchar_t*>(::RhRdkBuildDate()));
#endif

	// TODO: Add render plug-in initialization code here.

  // TODO: Add plug-in initialization code here.
  return CRhinoRenderPlugIn::OnLoadPlugIn();
}

void CSampleRdkRemoveUnusedContentPlugIn::OnUnloadPlugIn()
{
	// Description:
	//    Called one time when plug-in is about to be unloaded. By this time,
	//    Rhino's mainframe window has been destroyed, and some of the SDK
	//    managers have been deleted. There is also no active document or active
	//    view at this time. Thus, you should only be manipulating your own objects.
	//    or tools here.

	if (NULL != m_pRdkPlugIn)
	{
		m_pRdkPlugIn->Uninitialize();
		delete m_pRdkPlugIn;
		m_pRdkPlugIn = nullptr;
	}

	CRhinoRenderPlugIn::OnUnloadPlugIn();
}

CRhinoCommand::result CSampleRdkRemoveUnusedContentPlugIn::Render(const CRhinoCommandContext&, bool)
{
	return CRhinoCommand::failure;
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSampleRdkRemoveUnusedContentPlugIn::AddToPlugInHelpMenu() const
{
	// Description:
	//   Return true to have your plug-in name added to the Rhino help menu.
	//   OnDisplayPlugInHelp will be called when to activate your plug-in help.

	return FALSE;
}

BOOL CSampleRdkRemoveUnusedContentPlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
	// Description:
	//   Called when the user requests help about your plug-in.
	//   It should display a standard Windows Help file (.hlp or .chm).

	// TODO: Add support for online help here.
	return CRhinoRenderPlugIn::OnDisplayPlugInHelp(hWnd);
}
