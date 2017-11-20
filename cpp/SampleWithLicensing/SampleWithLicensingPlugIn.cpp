#include "StdAfx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleWithLicensingPlugIn.h"
#include "resource.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
#include "rhinoSdkPlugInDeclare.h"
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
RHINO_PLUG_IN_NAME(L"SampleWithLicensing");

// Rhino plug-in id
RHINO_PLUG_IN_ID(L"a8db70d4-f568-498f-968c-0129418e3c2d");

// Rhino plug-in version
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in
RHINO_PLUG_IN_DESCRIPTION(L"Rhino SDK Sample - SampleWithLicensing");

// Rhino plug-in icon resource id
// Provide the resource id of the plug-in icon.
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_MAIN);

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleWithLicensingPlugIn object
static CSampleWithLicensingPlugIn thePlugIn;

static CRhinoPlugIn::license_capabilities g_capabilities = (
  CRhinoPlugIn::license_capabilities)(CRhinoPlugIn::license_capabilities::can_be_evaluated
    | CRhinoPlugIn::license_capabilities::can_be_purchased
    | CRhinoPlugIn::license_capabilities::can_be_specified
    //| CRhinoPlugIn::license_capabilities::supports_rhino_accounts
    );

static wchar_t* g_text_mask = L"AAAAAAA-AAA-AAA";

/////////////////////////////////////////////////////////////////////////////
// CSampleWithLicensingPlugIn definition

CSampleWithLicensingPlugIn& SampleWithLicensingPlugIn()
{
  // Return a reference to the one and only CSampleWithLicensingPlugIn object
  return thePlugIn;
}

CSampleWithLicensingPlugIn::CSampleWithLicensingPlugIn()
{
  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleWithLicensingPlugIn::~CSampleWithLicensingPlugIn()
{
  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleWithLicensingPlugIn::PlugInName() const
{
  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleWithLicensingPlugIn::PlugInVersion() const
{
  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleWithLicensingPlugIn::PlugInID() const
{
  // TODO: Return a unique identifier for the plug-in.
  // {1F88325E-FA51-4B61-B3AC-27B589B70670}
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleWithLicensingPlugIn::OnLoadPlugIn()
{
  SetLicenseCapabilities(g_text_mask, g_capabilities, ON_UuidFromString(L"1ABD5EFC-2C65-11D7-B6F7-0050BABF6BC2"));
  
  // Ask Rhino to get a product license for us.
  bool rc = GetLicense();
  if (!rc)
    return -1; // Unable to initialize, unload the plug-in
               // and do not display load error dialog 

  return TRUE;
}

void CSampleWithLicensingPlugIn::OnUnloadPlugIn()
{
}

CRhinoPlugIn::plugin_load_time CSampleWithLicensingPlugIn::PlugInLoadTime()
{
  return plugin_load_time::load_plugin_at_startup;
}
