/////////////////////////////////////////////////////////////////////////////
// SampleContextMenuExtensionPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleContextMenuExtensionPlugIn.h"

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
RHINO_PLUG_IN_NAME( L"SampleContextMenuExtension" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"EDA14E59-9FC1-4F79-8DBB-95FF0F8889C1" );

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

// The one and only CSampleContextMenuExtensionPlugIn object
static CSampleContextMenuExtensionPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleContextMenuExtensionPlugIn definition

CSampleContextMenuExtensionPlugIn& SampleContextMenuExtensionPlugIn()
{ 
  // Return a reference to the one and only CSampleContextMenuExtensionPlugIn object
  return thePlugIn; 
}

CSampleContextMenuExtensionPlugIn::CSampleContextMenuExtensionPlugIn()
{
  // Description:
  //   CSampleContextMenuExtensionPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleContextMenuExtensionPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleContextMenuExtensionPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleContextMenuExtensionPlugIn::~CSampleContextMenuExtensionPlugIn()
{
  // Description:
  //   CSampleContextMenuExtensionPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleContextMenuExtensionPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleContextMenuExtensionPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleContextMenuExtensionPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleContextMenuExtensionPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {EDA14E59-9FC1-4F79-8DBB-95FF0F8889C1}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleContextMenuExtensionPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleContextMenuExtensionPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

CSampleLayerContextMenuExtension::CSampleLayerContextMenuExtension()
: CRhinoContextMenuExtension(thePlugIn, *thePlugIn.PlugInModuleState(), true)
{
  m_iAddItemID0 = m_iAddItemID1 = m_iAddItemID2 = -1;
}

CSampleLayerContextMenuExtension::~CSampleLayerContextMenuExtension()
{
}

bool CSampleLayerContextMenuExtension::ShouldExtendContextMenu( CRhinoContextMenuContext& context )
{
  // Extend the layer list's context menu
  if( 0 == ON_UuidCompare(context.m_uuid, CRhinoContextMenuExtension::UUIDLayerList()) )
    return true;
  return false;
}

void CSampleLayerContextMenuExtension::ExtendContextMenu( CRhinoContextMenuContext& context, CRhinoContextMenu& context_menu )
{
  m_iAddItemID0 = m_iAddItemID1 = m_iAddItemID2 = -1;
  if( 0 == ON_UuidCompare(context.m_uuid, CRhinoContextMenuExtension::UUIDLayerList()) )
  {
    m_iAddItemID0 = context_menu.AddItem( L"Sample Context Menu Item 1" );
    m_iAddItemID1 = context_menu.AddItem( L"Sample Context Menu Item 2" );
    m_iAddItemID2 = context_menu.AddItem( L"Sample Context Menu Item 3" );
  }
}

void CSampleLayerContextMenuExtension::OnCommand( CRhinoContextMenuContext& context, int iAddItemID, UINT nID, CRhinoContextMenu& context_menu )
{
  if( iAddItemID == m_iAddItemID0 )
    RhinoApp().Print( L"Sample Context Menu Item 1 selected.\n" );
  else if( iAddItemID == m_iAddItemID1 )
    RhinoApp().Print( L"Sample Context Menu Item 2 selected.\n" );
  else if( iAddItemID == m_iAddItemID2 )
    RhinoApp().Print( L"Sample Context Menu Item 3 selected.\n" );
}

void CSampleLayerContextMenuExtension::OnInitPopupMenu( CRhinoContextMenuContext& context, HWND hWnd, HMENU hMenuOriginal, HMENU hMenuRuntime, CRhinoContextMenu& context_menu )
{
  // TODO...
}
