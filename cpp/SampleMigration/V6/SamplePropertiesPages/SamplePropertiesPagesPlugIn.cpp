/////////////////////////////////////////////////////////////////////////////
// SamplePropertiesPagesPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SamplePropertiesPagesPlugIn.h"
#include "OptionsPage.h"
#include "DocumentPropertiesPage.h"

// Added for Rhino 6 Migration
#include "rhinoSdkPlugInDeclare.h"
// The plug-in object must be constructed before any plug-in classes
// derived from CRhinoCommand. The #pragma init_seg(lib) ensures that
// this happens.

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SamplePropertiesPages" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"FAE8F569-53C3-4956-9382-32FB776D893F" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__"  "__TIME__ )

// Rhino plug-in developer declarations
// TODO: fill in the following developer declarations with
// your company information. Note, all of these declarations
// must be present or your plug-in will not load.
//
// When completed, delete the following #error directive.
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION( L"My Company Name" );
RHINO_PLUG_IN_DEVELOPER_ADDRESS( L"123 Developer Street\r\nCity State 12345-6789" );
RHINO_PLUG_IN_DEVELOPER_COUNTRY( L"My Country" );
RHINO_PLUG_IN_DEVELOPER_PHONE( L"123.456.7890" );
RHINO_PLUG_IN_DEVELOPER_FAX( L"123.456.7891" );
RHINO_PLUG_IN_DEVELOPER_EMAIL( L"support@mycompany.com" );
RHINO_PLUG_IN_DEVELOPER_WEBSITE( L"http://www.mycompany.com" );
RHINO_PLUG_IN_UPDATE_URL( L"http://www.mycompany.com/support" );

// The one and only CSamplePropertiesPagesPlugIn object
static CSamplePropertiesPagesPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSamplePropertiesPagesPlugIn definition

CSamplePropertiesPagesPlugIn& SamplePropertiesPagesPlugIn()
{ 
  // Return a reference to the one and only CSamplePropertiesPagesPlugIn object
  return thePlugIn; 
}

CSamplePropertiesPagesPlugIn::CSamplePropertiesPagesPlugIn()
{
  // Description:
  //   CSamplePropertiesPagesPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSamplePropertiesPagesPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSamplePropertiesPagesPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSamplePropertiesPagesPlugIn::~CSamplePropertiesPagesPlugIn()
{
  // Description:
  //   CSamplePropertiesPagesPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSamplePropertiesPagesPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSamplePropertiesPagesPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSamplePropertiesPagesPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSamplePropertiesPagesPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {FAE8F569-53C3-4956-9382-32FB776D893F}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSamplePropertiesPagesPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSamplePropertiesPagesPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSamplePropertiesPagesPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return TRUE;
}

BOOL CSamplePropertiesPagesPlugIn::OnDisplayPlugInHelp( HWND hWnd ) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp( hWnd );
}

///////////////////////////////////////////////////////////////////////////////
// Extend object properties
void CSamplePropertiesPagesPlugIn::AddPagesToObjectPropertiesDialog(CRhinoPropertiesPanelPageCollection& collection)
{
  collection.Add(&m_properties_page);
}
///////////////////////////////////////////////////////////////////////////////
// Extend the Options dialog
void CSamplePropertiesPagesPlugIn::AddPagesToOptionsDialog(CRhinoOptionsPageCollection& collection)
{
  collection.AddPage(new COptionsPage());
}
///////////////////////////////////////////////////////////////////////////////
// Extend the Document Properties dialog
void CSamplePropertiesPagesPlugIn::AddPagesToDocumentPropertiesDialog(CRhinoOptionsPageCollection& collection)
{
  collection.AddPage(new CDocumentPropertiesPage());
}
