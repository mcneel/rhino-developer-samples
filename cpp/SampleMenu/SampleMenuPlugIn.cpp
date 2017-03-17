/////////////////////////////////////////////////////////////////////////////
// SampleMenuPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleMenuPlugIn.h"
#include "resource.h"

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
RHINO_PLUG_IN_NAME( L"SampleMenu" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"C6DB2BFC-8050-473D-9B53-A720E967CBCF" );

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

// The one and only CSampleMenuPlugIn object
static CSampleMenuPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleMenuPlugIn definition

CSampleMenuPlugIn& SampleMenuPlugIn()
{ 
  // Return a reference to the one and only CSampleMenuPlugIn object
  return thePlugIn; 
}

CSampleMenuPlugIn::CSampleMenuPlugIn()
{
  // Description:
  //   CSampleMenuPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleMenuPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleMenuPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
  m_bScriptMode = false;
}

CSampleMenuPlugIn::~CSampleMenuPlugIn()
{
  // Description:
  //   CSampleMenuPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleMenuPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleMenuPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleMenuPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleMenuPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {C6DB2BFC-8050-473D-9B53-A720E967CBCF}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleMenuPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  ShowSampleMenu();

  return TRUE;
}

void CSampleMenuPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSampleMenuPlugIn::AddToPlugInHelpMenu() const
{
  return TRUE;
}

BOOL CSampleMenuPlugIn::OnDisplayPlugInHelp( HWND hWnd ) const
{
  ::RhinoMessageBox( L"TODO: Add online help here.", PlugInName(), MB_OK );
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

void CSampleMenuPlugIn::OnInitPlugInMenuPopups( WPARAM wParam, LPARAM lParam )
{
  HMENU hMenu = (HMENU)wParam;
  if( hMenu )
  {
    UINT uCheck = MF_BYCOMMAND;
    if( m_bScriptMode )
      uCheck |= MF_CHECKED;
    else
      uCheck |= MF_UNCHECKED;
    ::CheckMenuItem( hMenu, ID_SAMPLEMENU_SCRIPTMODE, uCheck );
  }
}

BOOL CSampleMenuPlugIn::OnPlugInMenuCommand( WPARAM wParam )
{
  switch( wParam )
  {
  case ID_SAMPLEMENU_SAMPLEMENUCOMMAND1:
    {
      if( m_bScriptMode )
        RhinoApp().RunScript( L"! _-SampleMenuCommand1" );
      else
        RhinoApp().RunScript( L"! _SampleMenuCommand1" );
    }
    break;

  case ID_SAMPLEMENU_SAMPLEMENUCOMMAND2:
    {
      if( m_bScriptMode )
        RhinoApp().RunScript( L"! _-SampleMenuCommand2" );
      else
        RhinoApp().RunScript( L"! _SampleMenuCommand2" );
    }
    break;

  case ID_MORE_SAMPLEMENUCOMMAND3:
    {
      if( m_bScriptMode )
        RhinoApp().RunScript( L"! _-SampleMenuCommand3" );
      else
        RhinoApp().RunScript( L"! _SampleMenuCommand3" );
    }
    break;

  case ID_MORE_SAMPLEMENUCOMMAND4:
    {
      if( m_bScriptMode )
        RhinoApp().RunScript( L"! _-SampleMenuCommand4" );
      else
        RhinoApp().RunScript( L"! _SampleMenuCommand4" );
    }
    break;

  case ID_SAMPLEMENU_SCRIPTMODE:
    {
      if( m_bScriptMode )
        m_bScriptMode = false;
      else
        m_bScriptMode = true;
    }
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

BOOL CSampleMenuPlugIn::IsSampleMenuVisible() const
{
  return ( 0 != m_menu.GetSafeHmenu() );
}

BOOL CSampleMenuPlugIn::ShowSampleMenu()
{
  // Required to access our plug-in's resources
  AFX_MANAGE_STATE( AfxGetStaticModuleState() );

  BOOL rc = ( 0 == m_menu.GetSafeHmenu() );
  
  if( rc )
    rc = m_menu.LoadMenu( IDR_SAMPLE_MENU);

  if( rc )
  {
    // Insert the menu into Rhino's menu bar.
    // NOTE, this version gets a submenu from a main menu by index,
    // and uses the title in the main menu.
    InsertPlugInMenuToRhinoMenu( m_menu.GetSafeHmenu(), 0 );
  }

  return rc;
}

BOOL CSampleMenuPlugIn::HideSampleMenu()
{
  HMENU hSubMenu = 0;

  BOOL rc = ( 0 != m_menu.GetSafeHmenu() );
  
  if( rc )
  {
    // Since we used CRhinoPlugIn::InsertPlugInMenuToRhinoMenu to
    // insert a submenu, we need to retrieve the handle of the submenu
    // before we can remove it.

    MENUITEMINFO mi;
    memset( &mi, 0, sizeof(mi) );
    mi.cbSize = sizeof( mi );
    mi.fMask = MIIM_SUBMENU;
    rc = ::GetMenuItemInfo( m_menu.GetSafeHmenu(), 0, TRUE, &mi );

    if( rc )
      hSubMenu = mi.hSubMenu;
  }

  if( rc )
    rc = RemovePlugInMenuFromRhino( hSubMenu );

  if( rc )
    m_menu.DestroyMenu();

  return FALSE;
}
