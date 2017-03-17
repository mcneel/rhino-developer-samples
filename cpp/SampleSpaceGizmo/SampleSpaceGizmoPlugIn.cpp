#include "StdAfx.h"
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInDeclare.h"
#include "SampleSpaceGizmoPlugIn.h"
#include "SampleSpaceGizmoOptions.h"

// The plug-in object must be constructed before any plug-in classes derived
// from CRhinoCommand. The #pragma init_seg(lib) ensures that this happens.
#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME(L"SampleSpaceGizmo");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"6005555C-6C98-46A5-B4FF-A07D0F38AEFD");

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/Rhino6Samples_CPP");

// The one and only CSampleSpaceGizmoPlugIn object
static CSampleSpaceGizmoPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleSpaceGizmoPlugIn definition

CSampleSpaceGizmoPlugIn& SampleSpaceGizmoPlugIn()
{
  // Return a reference to the one and only CSampleSpaceGizmoPlugIn object
  return thePlugIn;
}

CSampleSpaceGizmoPlugIn::CSampleSpaceGizmoPlugIn()
  : m_pWnd(0)
{
  // Description:
  //   CSampleSpaceGizmoPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleSpaceGizmoPlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleSpaceGizmoPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleSpaceGizmoPlugIn::~CSampleSpaceGizmoPlugIn()
{
  // Description:
  //   CSampleSpaceGizmoPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleSpaceGizmoPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleSpaceGizmoPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleSpaceGizmoPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleSpaceGizmoPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {6005555C-6C98-46A5-B4FF-A07D0F38AEFD}
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleSpaceGizmoPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // Remarks:
  //    Plug-ins are not loaded until after Rhino is started and a default document
  //    is created.  Because the default document already exists
  //    CRhinoEventWatcher::On????Document() functions are not called for the default
  //    document.  If you need to do any document initialization/synchronization then
  //    override this function and do it here.  It is not necessary to call
  //    CPlugIn::OnLoadPlugIn() from your derived class.

  // TODO: Add plug-in initialization code here.

  // If we cannot connect to the Space Gizmo, just return a -1,
  // which will cause the plug-in not to load, but do so without
  // alerting the user.
  if (!CreateSpaceGizmoWnd())
    return -1;

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleSpaceGizmoPlugIn::OnUnloadPlugIn()
{
  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.

  // TODO: Add plug-in cleanup code here.

  DestroySpaceGizmoWnd();

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSampleSpaceGizmoPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return FALSE;
}

BOOL CSampleSpaceGizmoPlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

void CSampleSpaceGizmoPlugIn::LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  m_settings.LoadProfile(lpszSection, pc);
}

void CSampleSpaceGizmoPlugIn::SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  m_settings.SaveProfile(lpszSection, pc);
}

void CSampleSpaceGizmoPlugIn::AddPagesToOptionsDialog(HWND hWnd, ON_SimpleArray<CRhinoOptionsDialogPage*>& pages)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CSampleSpaceGizmoOptions* options_page = new CSampleSpaceGizmoOptions(CWnd::FromHandle(hWnd));
  if (options_page)
    pages.Append(options_page);
}

/////////////////////////////////////////////////////////////////////////////
// SpaceGizmo members

BOOL CSampleSpaceGizmoPlugIn::CreateSpaceGizmoWnd()
{
  // Create a message-only window that will receive the
  // "SpaceWareMessage00" registered window message

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (m_pWnd && ::IsWindow(m_pWnd->m_hWnd))
    return TRUE;

  if (m_pWnd)
  {
    delete m_pWnd;
    m_pWnd = 0;
  }

  m_pWnd = new CSampleSpaceGizmoWnd();

  ON_wString class_name = ::AfxRegisterWndClass(0);
  ON_wString window_name = RhinoPlugInId();
  BOOL rc = m_pWnd->CreateEx(0, class_name, window_name, WS_VISIBLE, 0, 0, 0, 0, HWND_MESSAGE, 0);
  if (!rc)
  {
    delete m_pWnd;
    m_pWnd = 0;
    return FALSE;
  }

  // Initialize the Space Gizmo
  rc = m_pWnd->InitializeSpaceGizmo();
  if (!rc)
  {
    if (::IsWindow(m_pWnd->m_hWnd))
      m_pWnd->DestroyWindow();
    delete m_pWnd;
    m_pWnd = 0;
    return FALSE;
  }

  return TRUE;
}

void CSampleSpaceGizmoPlugIn::DestroySpaceGizmoWnd()
{
  // Destroy the message-only window

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (m_pWnd)
  {
    if (::IsWindow(m_pWnd->m_hWnd))
      m_pWnd->DestroyWindow();
    delete m_pWnd;
  }

  m_pWnd = 0;
}

const CSampleSpaceGizmoSettings& CSampleSpaceGizmoPlugIn::SpaceGizmoSettings() const
{
  return m_settings;
}

void CSampleSpaceGizmoPlugIn::SetSpaceGizmoSettings(const CSampleSpaceGizmoSettings& settings)
{
  m_settings = settings;
}
