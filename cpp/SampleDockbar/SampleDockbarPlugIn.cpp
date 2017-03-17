/////////////////////////////////////////////////////////////////////////////
// SampleDockbarPlugIn.cpp
//

#include "StdAfx.h"
#include "SampleDockbarPlugIn.h"
#include "SampleRhinoDockbar.h"

#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME(L"SampleDockbar");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"C5411D4A-07FE-437C-BE7B-74840701E384");

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION(__DATE__"  "__TIME__)

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\015\012Seattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"tech@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/");

// The one and only CSampleDockbarPlugIn object
static CSampleDockbarPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleDockbarPlugIn definition

CSampleDockbarPlugIn& SampleDockbarPlugIn()
{ 
  return thePlugIn; 
}

CSampleDockbarPlugIn::CSampleDockbarPlugIn()
{
  m_plugin_version = RhinoPlugInVersion();
}

CSampleDockbarPlugIn::~CSampleDockbarPlugIn()
{
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleDockbarPlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSampleDockbarPlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CSampleDockbarPlugIn::PlugInID() const
{
  // {C5411D4A-07FE-437C-BE7B-74840701E384}
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleDockbarPlugIn::OnLoadPlugIn()
{
	// Ask Rhino's DockBarManager to create our dockbar
  CRhinoUiDockBar* pDockbar = RhinoUiDockBarManager().CreateRhinoDockBar(
        RUNTIME_CLASS(CSampleRhinoDockbar), 
        PlugInID(), 
        false, 
        AFX_IDW_DOCKBAR_FLOAT,
        CBRS_ALIGN_ANY,
        NULL,
        CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_LEFT
        );

  return TRUE;
}

void CSampleDockbarPlugIn::OnUnloadPlugIn()
{
}

