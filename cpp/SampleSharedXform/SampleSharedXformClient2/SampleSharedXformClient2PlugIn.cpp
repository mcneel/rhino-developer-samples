#include "StdAfx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleSharedXformClient2PlugIn.h"
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
RHINO_PLUG_IN_NAME(L"SampleSharedXformClient2");

// Rhino plug-in id
// Provide a unique uuid for this plug-in.
RHINO_PLUG_IN_ID(L"19FD060A-7659-4431-9EE0-B0C3ABD51159");

// Rhino plug-in version
// Provide a version number string for this plug-in.
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in.
RHINO_PLUG_IN_DESCRIPTION(L"SampleSharedXformClient2 plug-in for Rhinoceros®");

// Rhino plug-in icon resource id
// Provide an icon resource this plug-in.
// Icon resource should contain 16, 24, 32, 48, and 256-pixel image sizes.
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON);

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle, WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com/");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleSharedXformClient2PlugIn object
static class CSampleSharedXformClient2PlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleSharedXformClient2PlugIn definition

CSampleSharedXformClient2PlugIn& SampleSharedXformClient2PlugIn()
{
	// Return a reference to the one and only CSampleSharedXformClient2PlugIn object
	return thePlugIn;
}

CSampleSharedXformClient2PlugIn::CSampleSharedXformClient2PlugIn()
{
	// Description:
	//   CSampleSharedXformClient2PlugIn constructor. The constructor is called when the
	//   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	//   is loaded, CSampleSharedXformClient2PlugIn::OnLoadPlugIn() is called. The
	//   constructor should be simple and solid. Do anything that might fail in
	//   CSampleSharedXformClient2PlugIn::OnLoadPlugIn().

	// TODO: Add construction code here
	m_plugin_version = RhinoPlugInVersion();
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleSharedXformClient2PlugIn::PlugInName() const
{
	// Description:
	//   Plug-in name display string.  This name is displayed by Rhino when
	//   loading the plug-in, in the plug-in help menu, and in the Rhino
	//   interface for managing plug-ins.

	// TODO: Return a short, friendly name for the plug-in.
	return RhinoPlugInName();
}

const wchar_t* CSampleSharedXformClient2PlugIn::PlugInVersion() const
{
	// Description:
	//   Plug-in version display string. This name is displayed by Rhino
	//   when loading the plug-in and in the Rhino interface for managing
	//   plug-ins.

	// TODO: Return the version number of the plug-in.
	return m_plugin_version;
}

GUID CSampleSharedXformClient2PlugIn::PlugInID() const
{
	// Description:
	//   Plug-in unique identifier. The identifier is used by Rhino to
	//   manage the plug-ins.

	// TODO: Return a unique identifier for the plug-in.
	// {19FD060A-7659-4431-9EE0-B0C3ABD51159}
	return ON_UuidFromString(RhinoPlugInId());
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

BOOL CSampleSharedXformClient2PlugIn::OnLoadPlugIn()
{
	// Description:
	//   Called after the plug-in is loaded and the constructor has been
	//   run. This is a good place to perform any significant initialization,
	//   license checking, and so on.  This function must return TRUE for
	//   the plug-in to continue to load.

	// Remarks:
	//   Plug-ins are not loaded until after Rhino is started and a default document
	//   is created.  Because the default document already exists
	//   CRhinoEventWatcher::On????Document() functions are not called for the default
	//   document.  If you need to do any document initialization/synchronization then
	//   override this function and do it here.  It is not necessary to call
	//   CPlugIn::OnLoadPlugIn() from your derived class.

	// TODO: Add plug-in initialization code here.
	return TRUE;
}

void CSampleSharedXformClient2PlugIn::OnUnloadPlugIn()
{
	// Description:
	//   Called one time when plug-in is about to be unloaded. By this time,
	//   Rhino's mainframe window has been destroyed, and some of the SDK
	//   managers have been deleted. There is also no active document or active
	//   view at this time. Thus, you should only be manipulating your own objects.
	//   or tools here.

	// TODO: Add plug-in cleanup code here.
}
