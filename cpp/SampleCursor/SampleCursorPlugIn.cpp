/////////////////////////////////////////////////////////////////////////////
// SampleCursorPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleCursorPlugIn.h"
#include "resource.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SampleCursor" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"61823133-F976-4EF1-A693-3FB722B73077" );

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

// The one and only CSampleCursorPlugIn object
static CSampleCursorPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleCursorPlugIn definition

CSampleCursorPlugIn& SampleCursorPlugIn()
{ 
  // Return a reference to the one and only CSampleCursorPlugIn object
  return thePlugIn; 
}

CSampleCursorPlugIn::CSampleCursorPlugIn()
{
  m_plugin_version = RhinoPlugInVersion();
  m_hCursor = 0;
}

CSampleCursorPlugIn::~CSampleCursorPlugIn()
{
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleCursorPlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSampleCursorPlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CSampleCursorPlugIn::PlugInID() const
{
  // {61823133-F976-4EF1-A693-3FB722B73077}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleCursorPlugIn::OnLoadPlugIn()
{
  // By default, MFC uses the resource handle of the main application to load
  // the resource template. If you have an exported function in a DLL, such as
  // one that launches a dialog box in the DLL, this template is actually stored
  // in the DLL module. You need to switch the module state for the correct handle
  // to be used. You can do this by adding the following code to the beginning of 
  // the function. This swaps the current module state with the state returned from
  // AfxGetStaticModuleState until the end of the current scope.
  AFX_MANAGE_STATE( AfxGetStaticModuleState() );
  
  CWinApp* pApp = AfxGetApp();
  if( pApp )
    m_hCursor = pApp->LoadCursor( IDC_SAMPLE_CURSOR );

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleCursorPlugIn::OnUnloadPlugIn()
{
  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

HCURSOR CSampleCursorPlugIn::Cursor() const
{
  return m_hCursor;
}

