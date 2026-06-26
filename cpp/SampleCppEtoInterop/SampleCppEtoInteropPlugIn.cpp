// SampleCppEtoInteropPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "stdafx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleCppEtoInteropPlugIn.h"
#include "Resource.h"

#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
RHINO_PLUG_IN_NAME(L"SampleCppEtoInterop");

// Rhino plug-in id
RHINO_PLUG_IN_ID(L"DFD76B02-BFE6-4071-BEC6-EBA057C8CCC7");

// Rhino plug-in version
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
RHINO_PLUG_IN_DESCRIPTION(L"SampleCppEtoInterop plug-in for Rhinoceros®");

// Rhino plug-in icon resource id
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON);

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"146 North Canal Street, Suite 320\r\nSeattle, WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleCppEtoInteropPlugIn object
static class CSampleCppEtoInteropPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleCppEtoInteropPlugIn definition

CSampleCppEtoInteropPlugIn& SampleCppEtoInteropPlugIn()
{
  return thePlugIn;
}

CSampleCppEtoInteropPlugIn::CSampleCppEtoInteropPlugIn()
{
  m_plugin_version = RhinoPlugInVersion();
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleCppEtoInteropPlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSampleCppEtoInteropPlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CSampleCppEtoInteropPlugIn::PlugInID() const
{
  return ON_UuidFromString(RhinoPlugInId());
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

BOOL CSampleCppEtoInteropPlugIn::OnLoadPlugIn()
{
  return TRUE;
}

void CSampleCppEtoInteropPlugIn::OnUnloadPlugIn()
{
}
