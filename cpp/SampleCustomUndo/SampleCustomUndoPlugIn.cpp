/////////////////////////////////////////////////////////////////////////////
// SampleCustomUndoPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleCustomUndoPlugIn.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SampleCustomUndo" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"7A854C46-8F79-498C-B1AA-D2D65F270EC4" );

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

// The one and only CSampleCustomUndoPlugIn object
static CSampleCustomUndoPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleCustomUndoPlugIn definition

CSampleCustomUndoPlugIn& SampleCustomUndoPlugIn()
{ 
  // Return a reference to the one and only CSampleCustomUndoPlugIn object
  return thePlugIn; 
}

CSampleCustomUndoPlugIn::CSampleCustomUndoPlugIn()
{
  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
  m_balance = 0.0;
}

CSampleCustomUndoPlugIn::~CSampleCustomUndoPlugIn()
{
  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleCustomUndoPlugIn::PlugInName() const
{
  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleCustomUndoPlugIn::PlugInVersion() const
{
  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleCustomUndoPlugIn::PlugInID() const
{
  // TODO: Return a unique identifier for the plug-in.
  // {7A854C46-8F79-498C-B1AA-D2D65F270EC4}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleCustomUndoPlugIn::OnLoadPlugIn()
{
  m_watcher.Register();
  m_watcher.Enable();
  return TRUE;
}

void CSampleCustomUndoPlugIn::OnUnloadPlugIn()
{
}

double CSampleCustomUndoPlugIn::Balance() const
{
  return m_balance;
}

void CSampleCustomUndoPlugIn::SetBalance(double balance)
{
  m_balance = balance;
}
