#include "StdAfx.h"
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInDeclare.h"
#include "SampleUserInterfacePlugIn.h"
#include "SampleTabbedDockBarDialog.h"
#include "SampleScrollTabbedDockBarDialog.h"
#include "SampleOptionsPage.h"
#include "SampleDocumentPropertiesPage.h"
#include "SampleObjectPropertiesPageDialog.h"
#include "SampleRhinoDockbar.h"
#include "SampleObjectManagerDialog.h"
#include "SampleOptionsListCtrlPageDialog.h"

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
RHINO_PLUG_IN_NAME(L"SampleUserInterface");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"C952662D-0A32-4640-8FD4-D3CBAC79D96F");

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

// The one and only CSampleUserInterfacePlugIn object
static CSampleUserInterfacePlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleUserInterfacePlugIn definition

CSampleUserInterfacePlugIn& SampleUserInterfacePlugIn()
{
  // Return a reference to the one and only CSampleUserInterfacePlugIn object
  return thePlugIn;
}

CSampleUserInterfacePlugIn::CSampleUserInterfacePlugIn()
  : m_dialog(0)
  , m_extension_menu(0)
  , m_bScriptMode(true)
{
  // Description:
  //   CSampleUserInterfacePlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleUserInterfacePlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleUserInterfacePlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleUserInterfacePlugIn::~CSampleUserInterfacePlugIn()
{
  // Description:
  //   CSampleUserInterfacePlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleUserInterfacePlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here

  // Note, m_dialog is not deleted here, as the modeless
  // dialog will delete itself.

  if (nullptr != m_extension_menu)
  {
    delete m_extension_menu;
    m_extension_menu = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleUserInterfacePlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleUserInterfacePlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleUserInterfacePlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {C952662D-0A32-4640-8FD4-D3CBAC79D96F}
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleUserInterfacePlugIn::OnLoadPlugIn()
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

  // Register tabbed dockbar
  CSampleTabbedDockBarDialog::Register(RUNTIME_CLASS(CSampleTabbedDockBarDialog), CSampleTabbedDockBarDialog::IDD, AfxGetStaticModuleState());

  // Register scroll tabbed dockbar
  CSampleScrollTabbedDockBarDialog::Register(RUNTIME_CLASS(CSampleScrollTabbedDockBarDialog), CSampleScrollTabbedDockBarDialog::IDD, AfxGetStaticModuleState());

  // Register object manager panel
  CSampleObjectManagerDialog::CRhinoTabbedDockBarDialog::Register(RUNTIME_CLASS(CSampleObjectManagerDialog), CSampleObjectManagerDialog::IDD, AfxGetStaticModuleState());

  // Register list control panel
  CSampleOptionsListCtrlPageDialog::Register(RUNTIME_CLASS(CSampleOptionsListCtrlPageDialog), CSampleOptionsListCtrlPageDialog::IDD, AfxGetStaticModuleState());


  // Create our old-school dockbar
  RhinoUiDockBarManager().CreateRhinoDockBar(
    RUNTIME_CLASS(CSampleRhinoDockbar),
    PlugInID(),
    false,
    AFX_IDW_DOCKBAR_FLOAT,
    CBRS_ALIGN_ANY,
    NULL,
    CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_LEFT
  );

  // Extend the Layer panel's context menu
  m_extension_menu = new CSampleLayerContextMenuExtension(*this);

  // Show our sample menu on Rhino's main menu.
  // Note, the proper way of doing this today is to embed a menu
  // in a custom RUI using the toolbar workspace editor.
  ShowSampleMenu();

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleUserInterfacePlugIn::OnUnloadPlugIn()
{
  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSampleUserInterfacePlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return FALSE;
}

BOOL CSampleUserInterfacePlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// Modeless dialog helpers

bool CSampleUserInterfacePlugIn::IsDlgCreated()
{
  bool rc = false;
  if (m_dialog && ::IsWindow(m_dialog->m_hWnd))
    rc = true;
  return rc;
}

bool CSampleUserInterfacePlugIn::IsDlgVisible()
{
  bool rc = false;
  if (IsDlgCreated() && m_dialog->IsWindowVisible())
    rc = true;
  return rc;

}

bool CSampleUserInterfacePlugIn::SetDlgVisible()
{
  bool rc = false;
  if (IsDlgCreated() && !IsDlgVisible())
  {
    m_dialog->ShowWindow(SW_SHOWNORMAL);
    m_dialog->SetFocus();
    rc = true;
  }
  return rc;
}

bool CSampleUserInterfacePlugIn::SetDlgHidden()
{
  bool rc = false;
  if (IsDlgCreated() && IsDlgVisible())
  {
    m_dialog->ShowWindow(SW_HIDE);
    ::SetFocus(RhinoApp().MainWnd());
    rc = true;
  }
  return rc;
}

bool CSampleUserInterfacePlugIn::DisplayDlg()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (IsDlgCreated())
  {
    if (!IsDlgVisible())
      SetDlgVisible();
    m_dialog->SetFocus();
    return true;
  }

  m_dialog = new CSampleModelessDialog();
  if (m_dialog->Create(IDD_MODELESS_DIALOG, CWnd::FromHandle(RhinoApp().MainWnd())))
  {
    m_dialog->ShowWindow(SW_SHOW);
    m_dialog->UpdateWindow();
    m_dialog->SetFocus();
    return true;
  }

  return false;
}

void CSampleUserInterfacePlugIn::DestroyDlg()
{
  if (IsDlgCreated())
  {
    m_dialog->KillDialog();
    m_dialog = 0;
  }
}

void CSampleUserInterfacePlugIn::ZeroDlg()
{
  m_dialog = 0;
}

void CSampleUserInterfacePlugIn::SetDlgPointValue(int item, const ON_3dPoint& pt)
{
  if (IsDlgVisible())
    m_dialog->SetPointValue(item, pt);
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

CRhinoPlugIn::plugin_load_time CSampleUserInterfacePlugIn::PlugInLoadTime()
{
  return CRhinoPlugIn::load_plugin_when_needed_or_optionsdlg;
}

void CSampleUserInterfacePlugIn::AddPagesToOptionsDialog(HWND hWnd, ON_SimpleArray<class CRhinoOptionsDialogPage*>& pages)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  CSampleOptionsPage* pPage = new CSampleOptionsPage(CWnd::FromHandle(hWnd));
  if (pPage)
    pages.Append(pPage);
}

void CSampleUserInterfacePlugIn::AddPagesToDocumentPropertiesDialog(CRhinoDoc& doc, HWND hWnd, ON_SimpleArray<class CRhinoOptionsDialogPage*>& pages)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  UNREFERENCED_PARAMETER(doc);

  CSampleDocumentPropertiesPage* pPage = new CSampleDocumentPropertiesPage(CWnd::FromHandle(hWnd));
  if (pPage)
    pages.Append(pPage);
}

void CSampleUserInterfacePlugIn::AddPagesToObjectPropertiesDialog(ON_SimpleArray<class CRhinoPropertiesDialogPage*>& pages)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  HWND hWnd = RhinoApp().MainWnd();
  CSampleObjectPropertiesPageDialog* pPage = new CSampleObjectPropertiesPageDialog(IDD_OBJPROPS_DIALOG, CWnd::FromHandle(hWnd));
  if (pPage)
    pages.Append(pPage);
}

void CSampleUserInterfacePlugIn::OnInitPlugInMenuPopups(WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);

  HMENU hMenu = (HMENU)wParam;
  if (hMenu)
  {
    UINT uCheck = MF_BYCOMMAND;
    if (m_bScriptMode)
      uCheck |= MF_CHECKED;
    else
      uCheck |= MF_UNCHECKED;
    ::CheckMenuItem(hMenu, ID_SAMPLEMENU_SCRIPTMODE, uCheck);
  }
}

BOOL CSampleUserInterfacePlugIn::OnPlugInMenuCommand(WPARAM wParam)
{
  switch (wParam)
  {
  case ID_SAMPLEMENU_SAMPLEMENUCOMMAND1:
  {
    if (m_bScriptMode)
      RhinoApp().RunScript(L"! _-SampleMenuCommand1");
    else
      RhinoApp().RunScript(L"! _SampleMenuCommand1");
  }
  break;

  case ID_SAMPLEMENU_SAMPLEMENUCOMMAND2:
  {
    if (m_bScriptMode)
      RhinoApp().RunScript(L"! _-SampleMenuCommand2");
    else
      RhinoApp().RunScript(L"! _SampleMenuCommand2");
  }
  break;

  case ID_MORE_SAMPLEMENUCOMMAND3:
  {
    if (m_bScriptMode)
      RhinoApp().RunScript(L"! _-SampleMenuCommand3");
    else
      RhinoApp().RunScript(L"! _SampleMenuCommand3");
  }
  break;

  case ID_MORE_SAMPLEMENUCOMMAND4:
  {
    if (m_bScriptMode)
      RhinoApp().RunScript(L"! _-SampleMenuCommand4");
    else
      RhinoApp().RunScript(L"! _SampleMenuCommand4");
  }
  break;

  case ID_SAMPLEMENU_SCRIPTMODE:
  {
    if (m_bScriptMode)
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

BOOL CSampleUserInterfacePlugIn::IsSampleMenuVisible() const
{
  return (0 != m_menu.GetSafeHmenu());
}

BOOL CSampleUserInterfacePlugIn::ShowSampleMenu()
{
  // Required to access our plug-in's resources
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  BOOL rc = (0 == m_menu.GetSafeHmenu());

  if (rc)
    rc = m_menu.LoadMenu(IDR_SAMPLE_MENU);

  if (rc)
  {
    // Insert the menu into Rhino's menu bar.
    // NOTE, this version gets a submenu from a main menu by index,
    // and uses the title in the main menu.
    InsertPlugInMenuToRhinoMenu(m_menu.GetSafeHmenu(), 0);
  }

  return rc;
}

BOOL CSampleUserInterfacePlugIn::HideSampleMenu()
{
  HMENU hSubMenu = 0;

  BOOL rc = (0 != m_menu.GetSafeHmenu());

  if (rc)
  {
    // Since we used CRhinoPlugIn::InsertPlugInMenuToRhinoMenu to
    // insert a submenu, we need to retrieve the handle of the submenu
    // before we can remove it.

    MENUITEMINFO mi;
    memset(&mi, 0, sizeof(mi));
    mi.cbSize = sizeof(mi);
    mi.fMask = MIIM_SUBMENU;
    rc = ::GetMenuItemInfo(m_menu.GetSafeHmenu(), 0, TRUE, &mi);

    if (rc)
      hSubMenu = mi.hSubMenu;
  }

  if (rc)
    rc = RemovePlugInMenuFromRhino(hSubMenu);

  if (rc)
    m_menu.DestroyMenu();

  return FALSE;
}

bool CSampleUserInterfacePlugIn::ScriptMode() const
{
  return m_bScriptMode;
}

void CSampleUserInterfacePlugIn::SetScriptMode(bool bSet)
{
  m_bScriptMode = bSet;
}
