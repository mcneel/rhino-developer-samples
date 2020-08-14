// SampleRealtimeRendererPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleRealtimeRendererPlugIn.h"
#include "SampleDisplayMode.h"
#include "Resource.h"

// RhRdk::Realtime::DisplayMode::Factory is missing its virtual destructor and to fix this would break the RDK SDK. So...
#pragma warning(disable : 5205) // delete of an abstract class that has a non-virtual destructor results in undefined behavior

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
RHINO_PLUG_IN_NAME(L"SampleRealtimeRenderer");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"063A34A5-4D31-401D-B0CD-692FDB089B06");

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in
RHINO_PLUG_IN_DESCRIPTION(L"Rhino SDK Sample - SampleRealtimeRenderer");

// Rhino plug-in icon resource id
// Provide the resource id of the plug-in icon.
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

// The one and only CSampleRealtimeRendererPlugIn object
static CSampleRealtimeRendererPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleRealtimeRendererPlugIn definition

CSampleRealtimeRendererPlugIn& SampleRealtimeRendererPlugIn()
{
	// Return a reference to the one and only CSampleRealtimeRendererPlugIn object
	return thePlugIn;
}

CSampleRealtimeRendererPlugIn::CSampleRealtimeRendererPlugIn()
{
	// Description:
	//   CSampleRealtimeRendererPlugIn constructor. The constructor is called when the
	//   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	//   is loaded, CSampleRealtimeRendererPlugIn::OnLoadPlugIn() is called. The
	//   constructor should be simple and solid. Do anything that might fail in
	//   CSampleRealtimeRendererPlugIn::OnLoadPlugIn().

	// TODO: Add construction code here
	m_plugin_version = RhinoPlugInVersion();
}

CSampleRealtimeRendererPlugIn::~CSampleRealtimeRendererPlugIn()
{
	// Description:
	//   CSampleRealtimeRendererPlugIn destructor. The destructor is called to destroy
	//   "thePlugIn" when the plug-in is unloaded. Immediately before the
	//   DLL is unloaded, CSampleRealtimeRendererPlugIn::OnUnloadPlugin() is called. Do
	//   not do too much here. Be sure to clean up any memory you have allocated
	//   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

	// TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleRealtimeRendererPlugIn::PlugInName() const
{
	// Description:
	//   Plug-in name display string.  This name is displayed by Rhino when
	//   loading the plug-in, in the plug-in help menu, and in the Rhino
	//   interface for managing plug-ins.

	// TODO: Return a short, friendly name for the plug-in.
	return RhinoPlugInName();
}

const wchar_t* CSampleRealtimeRendererPlugIn::PlugInVersion() const
{
	// Description:
	//   Plug-in version display string. This name is displayed by Rhino
	//   when loading the plug-in and in the Rhino interface for managing
	//   plug-ins.

	// TODO: Return the version number of the plug-in.
	return m_plugin_version;
}

GUID CSampleRealtimeRendererPlugIn::PlugInID() const
{
	// Description:
	//   Plug-in unique identifier. The identifier is used by Rhino to
	//   manage the plug-ins.

	// TODO: Return a unique identifier for the plug-in.
	// {336403E8-9B17-41C5-910F-7D951D99E607}
	return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleRealtimeRendererPlugIn::OnLoadPlugIn()
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


	// Register display mode factory
	RhRdk::Realtime::DisplayMode::Factory::Register(std::make_unique<CDisplayModeFactory>());

	//// Look if the display mode is already added...
	DisplayAttrsMgrListDesc* pDisplayAttrsMgrListDesc = CRhinoDisplayAttrsMgr::FindDisplayAttrsDesc(DisplayModeId());
	// ... and if it isn't then add it.
	if (0 == pDisplayAttrsMgrListDesc)
	{
		pDisplayAttrsMgrListDesc = CRhinoDisplayAttrsMgr::AppendNewEntry();
		if (0 != pDisplayAttrsMgrListDesc && 0 != pDisplayAttrsMgrListDesc->m_pAttrs)
		{
			// Show it in the drop down menu
			pDisplayAttrsMgrListDesc->m_bAddToMenu = true;

			// Set it's guid, name and pipeline class
			pDisplayAttrsMgrListDesc->m_pAttrs->SetUuid(DisplayModeId());
			pDisplayAttrsMgrListDesc->m_pAttrs->SetName(PlugInName());
			pDisplayAttrsMgrListDesc->m_pAttrs->SetRealtimeDisplayId(DisplayModeId());
			pDisplayAttrsMgrListDesc->m_pAttrs->SetPipeline(&ON_CLASS_RTTI(CRhinoDisplayPipeline_OGL));
			// Save changes
			CRhinoDisplayAttrsMgr::SaveProfile(RhinoApp().ProfileContext());
		}
	}

	return TRUE;
}

void CSampleRealtimeRendererPlugIn::OnUnloadPlugIn()
{
	// Description:
	//    Called one time when plug-in is about to be unloaded. By this time,
	//    Rhino's mainframe window has been destroyed, and some of the SDK
	//    managers have been deleted. There is also no active document or active
	//    view at this time. Thus, you should only be manipulating your own objects.
	//    or tools here.

	// TODO: Add plug-in cleanup code here.
}

/////////////////////////////////////////////////////////////////////////////
// Load time override

CRhinoPlugIn::plugin_load_time CSampleRealtimeRendererPlugIn::PlugInLoadTime()
{
	// This plugin has to be loaded at startup in order to register the pipeline.
	return CRhinoPlugIn::load_plugin_at_startup;
}
