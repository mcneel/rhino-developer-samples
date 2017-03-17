/////////////////////////////////////////////////////////////////////////////
// SampleDialogsPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleDialogsPlugIn.h"

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
RHINO_PLUG_IN_NAME( L"SampleDialogs" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"CCF3FBCD-4A09-4EB3-A9A3-55AE9662DD2E" );

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

// The one and only CSampleDialogsPlugIn object
static CSampleDialogsPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleDialogsPlugIn definition

CSampleDialogsPlugIn& SampleDialogsPlugIn()
{ 
  // Return a reference to the one and only CSampleDialogsPlugIn object
  return thePlugIn; 
}

CSampleDialogsPlugIn::CSampleDialogsPlugIn()
  : m_dialog(0)
{
  // Description:
  //   CSampleDialogsPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleDialogsPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleDialogsPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleDialogsPlugIn::~CSampleDialogsPlugIn()
{
  // Description:
  //   CSampleDialogsPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleDialogsPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleDialogsPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleDialogsPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleDialogsPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {CCF3FBCD-4A09-4EB3-A9A3-55AE9662DD2E}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleDialogsPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleDialogsPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

bool CSampleDialogsPlugIn::IsDlgCreated()
{
  bool rc = false;
  if( m_dialog && ::IsWindow(m_dialog->m_hWnd) )
    rc = true;
  return rc;
}

bool CSampleDialogsPlugIn::IsDlgVisible()
{
  bool rc = false;
  if( IsDlgCreated() && m_dialog->IsWindowVisible() )
    rc = true;
  return rc;

}

bool CSampleDialogsPlugIn::SetDlgVisible()
{
  bool rc = false;
  if( IsDlgCreated() && !IsDlgVisible() )
  {
    m_dialog->ShowWindow( SW_SHOWNORMAL );
    m_dialog->SetFocus();
    rc = true;
  }
  return rc;
}

bool CSampleDialogsPlugIn::SetDlgHidden()
{
  bool rc = false;
  if( IsDlgCreated() && IsDlgVisible() )
  {
    m_dialog->ShowWindow( SW_HIDE );
    ::SetFocus( RhinoApp().MainWnd() );
    rc = true;
  }
  return rc;
}

bool CSampleDialogsPlugIn::DisplayDlg()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

  if( IsDlgCreated() )
  {
    if( !IsDlgVisible() )
      SetDlgVisible();
    m_dialog->SetFocus();
    return true;
  }

  m_dialog = new CSampleModelessDialog();
	if( m_dialog->Create(IDD_MODELESS_DIALOG, CWnd::FromHandle(RhinoApp().MainWnd())) )
	{
		m_dialog->ShowWindow( SW_SHOW );
		m_dialog->UpdateWindow();
    m_dialog->SetFocus();
		return true;
	}

	return false;
}

void CSampleDialogsPlugIn::DestroyDlg()
{
  if( IsDlgCreated() )
  {
    m_dialog->KillDialog();
    m_dialog = 0;
  }
}

void CSampleDialogsPlugIn::ZeroDlg()
{
  m_dialog = 0;
}

void CSampleDialogsPlugIn::SetDlgPointValue( int item, const ON_3dPoint& pt )
{
  if( IsDlgVisible() )
    m_dialog->SetPointValue( item, pt );
}


