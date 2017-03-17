/////////////////////////////////////////////////////////////////////////////
// SampleClippingPlanesPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleClippingPlanesPlugIn.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SampleClippingPlanes" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"4C632536-D30B-4DB8-8F4A-5E8899674FD0" );

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

// The one and only CSampleClippingPlanesPlugIn object
static CSampleClippingPlanesPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleClippingPlanesPlugIn definition

CSampleClippingPlanesPlugIn& SampleClippingPlanesPlugIn()
{ 
  // Return a reference to the one and only CSampleClippingPlanesPlugIn object
  return thePlugIn; 
}

CSampleClippingPlanesPlugIn::CSampleClippingPlanesPlugIn()
{
  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleClippingPlanesPlugIn::~CSampleClippingPlanesPlugIn()
{
  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleClippingPlanesPlugIn::PlugInName() const
{
  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleClippingPlanesPlugIn::PlugInVersion() const
{
  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleClippingPlanesPlugIn::PlugInID() const
{
  // TODO: Return a unique identifier for the plug-in.
  // {4C632536-D30B-4DB8-8F4A-5E8899674FD0}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleClippingPlanesPlugIn::OnLoadPlugIn()
{
  // TODO: Add plug-in initialization code here.
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleClippingPlanesPlugIn::OnUnloadPlugIn()
{
  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

int CSampleClippingPlanesPlugIn::StandardViewportIds( CRhinoDoc& doc, ON_SimpleArray<UUID>& viewport_ids )
{
  ON_SimpleArray<CRhinoView*> view_list;
  const int view_count = doc.GetViewList( view_list, true, false );
  for( int i = 0; i < view_count; i++ )
  {
    CRhinoView* view = view_list[i];
    if( view )
      viewport_ids.Append( view->ActiveViewportID() );
  }
  return viewport_ids.Count();
}
