/////////////////////////////////////////////////////////////////////////////
// SampleOptionsListCtrlPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleOptionsListCtrlPlugIn.h"
#include "SampleOptionsListCtrlDlg.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
RHINO_PLUG_IN_NAME( L"SampleOptionsListCtrl" );

// Rhino plug-in id
RHINO_PLUG_IN_ID( L"9E0EEC4B-5771-42B4-8205-1B31568A792A" );

// Rhino plug-in version
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

// The one and only CSampleOptionsListCtrlPlugIn object
static CSampleOptionsListCtrlPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleOptionsListCtrlPlugIn definition

CSampleOptionsListCtrlPlugIn& SampleOptionsListCtrlPlugIn()
{ 
  // Return a reference to the one and only CSampleOptionsListCtrlPlugIn object
  return thePlugIn; 
}

CSampleOptionsListCtrlPlugIn::CSampleOptionsListCtrlPlugIn()
{
  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleOptionsListCtrlPlugIn::~CSampleOptionsListCtrlPlugIn()
{
  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleOptionsListCtrlPlugIn::PlugInName() const
{
  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleOptionsListCtrlPlugIn::PlugInVersion() const
{
  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleOptionsListCtrlPlugIn::PlugInID() const
{
  // TODO: Return a unique identifier for the plug-in.
  // {9E0EEC4B-5771-42B4-8205-1B31568A792A}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleOptionsListCtrlPlugIn::OnLoadPlugIn()
{
  // TODO: Add plug-in initialization code here.

  CSampleOptionsListCtrlDlg::Register( 
    RUNTIME_CLASS(CSampleOptionsListCtrlDlg), 
    CSampleOptionsListCtrlDlg::IDD, 
    AfxGetStaticModuleState() 
    );

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleOptionsListCtrlPlugIn::OnUnloadPlugIn()
{
  // TODO: Add plug-in cleanup code here.
  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

