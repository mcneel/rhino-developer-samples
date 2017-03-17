/////////////////////////////////////////////////////////////////////////////
// SampleTabDialogPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleTabDialogPlugIn.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SampleTabDialog" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"5C2CC24C-0EC3-4E06-B9E7-5BA2092C7226" );

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


// The one and only CSampleTabDialogPlugIn object
static CSampleTabDialogPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleTabDialogPlugIn definition

CSampleTabDialogPlugIn& SampleTabDialogPlugIn()
{ 
  // Return a reference to the one and only CSampleTabDialogPlugIn object
  return thePlugIn; 
}

CSampleTabDialogPlugIn::CSampleTabDialogPlugIn()
{
  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleTabDialogPlugIn::~CSampleTabDialogPlugIn()
{
  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleTabDialogPlugIn::PlugInName() const
{
  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleTabDialogPlugIn::PlugInVersion() const
{
  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleTabDialogPlugIn::PlugInID() const
{
  // TODO: Return a unique identifier for the plug-in.
  // {5C2CC24C-0EC3-4E06-B9E7-5BA2092C7226}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleTabDialogPlugIn::OnLoadPlugIn()
{
  // TODO: Add plug-in initialization code here.
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleTabDialogPlugIn::OnUnloadPlugIn()
{
  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

