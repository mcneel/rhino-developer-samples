/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoPlugIn.cpp

#include "StdAfx.h"
#include "SpaceGizmoPlugIn.h"
#include "SpaceGizmoOptions.h"

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
RHINO_PLUG_IN_NAME( L"SpaceGizmo" );

// Rhino plug-in id
RHINO_PLUG_IN_ID( L"D3158019-2EDA-4CE2-A2D2-D0B454FFE638" );

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

// The one and only CSpaceGizmoPlugIn object
static CSpaceGizmoPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSpaceGizmoPlugIn definition

CSpaceGizmoPlugIn& SpaceGizmoPlugIn()
{ 
  // Return a reference to the one and only CSpaceGizmoPlugIn object
  return thePlugIn; 
}

CSpaceGizmoPlugIn::CSpaceGizmoPlugIn()
  : m_pWnd(0)
{
  m_plugin_version = RhinoPlugInVersion();
}

CSpaceGizmoPlugIn::~CSpaceGizmoPlugIn()
{
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSpaceGizmoPlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSpaceGizmoPlugIn::LocalPlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSpaceGizmoPlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CSpaceGizmoPlugIn::PlugInID() const
{
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSpaceGizmoPlugIn::OnLoadPlugIn()
{
  // If we cannot connect to the Space Gizmo, just return a -1,
  // which will cause the plug-in not to load, but do so without
  // alerting the user.
  if( !CreateSpaceGizmoWnd() )
    return -1;

  return TRUE;
}

void CSpaceGizmoPlugIn::OnUnloadPlugIn()
{
  DestroySpaceGizmoWnd();
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides
  
void CSpaceGizmoPlugIn::LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc )
{
  m_settings.LoadProfile( lpszSection, pc );
}

void CSpaceGizmoPlugIn::SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc )
{
  m_settings.SaveProfile( lpszSection, pc );
}

void CSpaceGizmoPlugIn::AddPagesToOptionsDialog( HWND hWnd, ON_SimpleArray<CRhinoOptionsDialogPage*>& pages )
{
  AFX_MANAGE_STATE( AfxGetStaticModuleState() );
  CSpaceGizmoOptions* options_page = new CSpaceGizmoOptions( CWnd::FromHandle(hWnd) );
  if( options_page )
    pages.Append( options_page );
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSpaceGizmoPlugIn::AddToPlugInHelpMenu() const
{
  return TRUE;
}

BOOL CSpaceGizmoPlugIn::OnDisplayPlugInHelp( HWND hWnd ) const
{
  MessageBox( RhinoApp().MainWnd(), L"Online help under construction", LocalPlugInName(), MB_OK | MB_ICONASTERISK );
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// SpaceGizmo members

BOOL CSpaceGizmoPlugIn::CreateSpaceGizmoWnd()
{
  // Create a message-only window that will receive the
  // "SpaceWareMessage00" registered window message

  AFX_MANAGE_STATE( AfxGetStaticModuleState() );

  if( m_pWnd && ::IsWindow(m_pWnd->m_hWnd) )
    return TRUE;

  if( m_pWnd )
  {
    delete m_pWnd;
    m_pWnd = 0;
  }

  m_pWnd = new CSpaceGizmoWnd();
  
  ON_wString class_name = ::AfxRegisterWndClass( 0 );
  ON_wString window_name = RhinoPlugInId();
  BOOL rc = m_pWnd->CreateEx( 0, class_name, window_name, WS_VISIBLE, 0, 0, 0, 0, HWND_MESSAGE, 0 );
  if( !rc )
  {
    delete m_pWnd;
    m_pWnd = 0;
    return FALSE;
  }

  // Initialize the Space Gizmo
  rc = m_pWnd->InitializeSpaceGizmo();
  if( !rc )
  {
    if( ::IsWindow(m_pWnd->m_hWnd) )
      m_pWnd->DestroyWindow();
    delete m_pWnd;
    m_pWnd = 0;
    return FALSE;
  }

  return TRUE;
}

void CSpaceGizmoPlugIn::DestroySpaceGizmoWnd()
{
  // Destroy the message-only window

  AFX_MANAGE_STATE( AfxGetStaticModuleState() );

  if( m_pWnd )
  {
    if( ::IsWindow(m_pWnd->m_hWnd) )
      m_pWnd->DestroyWindow();
    delete m_pWnd;
  }

  m_pWnd = 0;
}

const CSpaceGizmoSettings& CSpaceGizmoPlugIn::SpaceGizmoSettings() const
{
  return m_settings;
}

void CSpaceGizmoPlugIn::SetSpaceGizmoSettings( const CSpaceGizmoSettings& settings )
{
  m_settings = settings;
}
