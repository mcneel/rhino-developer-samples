/////////////////////////////////////////////////////////////////////////////
// SampleScrollTabbedDockBarPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleScrollTabbedDockBarPlugIn.h"
#include "SampleScrollTabbedDockBarDialog.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SampleScrollTabbedDockBar" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"068B63F1-7DB8-43FF-ACF7-FE5FD40DE241" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__"  "__TIME__ )

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION( L"Robert McNeel & Associates" );
RHINO_PLUG_IN_DEVELOPER_ADDRESS( L"3670 Woodland Park Avenue North\015\012Seattle WA 98103" );
RHINO_PLUG_IN_DEVELOPER_COUNTRY( L"United States" );
RHINO_PLUG_IN_DEVELOPER_PHONE( L"206-545-6877" );
RHINO_PLUG_IN_DEVELOPER_FAX( L"206-545-7321" );
RHINO_PLUG_IN_DEVELOPER_EMAIL( L"tech@mcneel.com" );
RHINO_PLUG_IN_DEVELOPER_WEBSITE( L"http://www.rhino3d.com" );
RHINO_PLUG_IN_UPDATE_URL( L"https://github.com/mcneel/" );

// The one and only CSampleScrollTabbedDockBarPlugIn object
static CSampleScrollTabbedDockBarPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleScrollTabbedDockBarPlugIn definition

CSampleScrollTabbedDockBarPlugIn& SampleScrollTabbedDockBarPlugIn()
{ 
  // Return a reference to the one and only CSampleScrollTabbedDockBarPlugIn object
  return thePlugIn; 
}

CSampleScrollTabbedDockBarPlugIn::CSampleScrollTabbedDockBarPlugIn()
{
  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleScrollTabbedDockBarPlugIn::~CSampleScrollTabbedDockBarPlugIn()
{
  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleScrollTabbedDockBarPlugIn::PlugInName() const
{
  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleScrollTabbedDockBarPlugIn::PlugInVersion() const
{
  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleScrollTabbedDockBarPlugIn::PlugInID() const
{
  // TODO: Return a unique identifier for the plug-in.
  // {068B63F1-7DB8-43FF-ACF7-FE5FD40DE241}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleScrollTabbedDockBarPlugIn::OnLoadPlugIn()
{
  // TODO: Add plug-in initialization code here.
  CSampleScrollTabbedDockBarDialog::Register( 
    RUNTIME_CLASS(CSampleScrollTabbedDockBarDialog), 
    CSampleScrollTabbedDockBarDialog::IDD, 
    AfxGetStaticModuleState() 
    );

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleScrollTabbedDockBarPlugIn::OnUnloadPlugIn()
{
  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

