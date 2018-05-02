
#include "stdafx.h"
#include "MarmaladePlugIn.h"
#include "Resource.h"
#include "MarmaladeSdkRender.h"
#include "MarmaladeRdkPlugIn.h"
#include "MarmaladeNonModalOptionsDlg.h"
#include "MarmaladeViewPropertiesPage.h"

// The plug-in object must be constructed before any plug-in classes
// derived from CRhinoCommand. The #pragma init_seg(lib) ensures that
// this happens.

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// rhinoSdkPlugInDeclare.h defines the RHINO_PLUG_IN_DECLARE macro
#include "C:/Program Files/Rhino 6 SDK/Inc/rhinoSdkPlugInDeclare.h"
RHINO_PLUG_IN_DECLARE

RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS     (L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY     (L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE       (L"206-545-7000");
RHINO_PLUG_IN_DEVELOPER_FAX         (L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL       (L"tech@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE     (L"http://www.mcneel.com");
RHINO_PLUG_IN_UPDATE_URL            (L"http://www.mcneel.com");

// The one and only CMarmaladePlugIn object
static CMarmaladePlugIn thePlugIn;

void CALLBACK EXPORT TimerProc(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	MarmaladePlugIn().UpdateMarmaladeMenuState();
}

const UINT iTimerId = 0x4576343;

CMarmaladePlugIn& MarmaladePlugIn()
{ 
	// Return a reference to the one and only CMarmaladePlugIn object
	return thePlugIn; 
}

CMarmaladePlugIn::CMarmaladePlugIn()
{
	// Description:
	//   CMarmaladePlugIn constructor. The constructor is called when the
	//   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	//   is loaded, CMarmaladePlugIn::OnLoadPlugIn() is called. The 
	//   constructor should be simple and solid. Do anything that might fail in
	//   CMarmaladePlugIn::OnLoadPlugIn().

	m_plugin_version = __DATE__ "  " __TIME__;
}

CMarmaladePlugIn::~CMarmaladePlugIn()
{
	// Description:
	//   CMarmaladePlugIn destructor. The destructor is called to destroy
	//   "thePlugIn" when the plug-in is unloaded. Immediately before the
	//   DLL is unloaded, CMarmaladePlugIn::OnUnloadPlugin() is called. Do
	//   not do too much here. Be sure to clean up any memory you have allocated
	//   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

	delete m_pMenu;
	m_pMenu = nullptr;
}

const wchar_t* CMarmaladePlugIn::PlugInName() const
{
	// Description:
	//   Plug-in name display string.  This name is displayed by Rhino when
	//   loading the plug-in, in the plug-in help menu, and in the Rhino 
	//   interface for managing plug-ins.

	// TODO: Return a short, friendly name for the plug-in.
	return L"Marmalade";
}

const wchar_t* CMarmaladePlugIn::PlugInVersion() const
{
	// Description:
	//   Plug-in version display string. This name is displayed by Rhino 
	//   when loading the plug-in and in the Rhino interface for managing
	//   plug-ins.

	// TODO: Return the version number of the plug-in.
	return m_plugin_version;
}

GUID CMarmaladePlugIn::PlugInID() const
{
	return ID();
}

GUID CMarmaladePlugIn::ID() // Static.
{
	// Description:
	//   Plug-in unique identifier. The identifier is used by Rhino to
	//   manage the plug-ins.

	// TODO: Return a unique identifier for the plug-in.

	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {B1BADD73-A606-4CB7-834C-54177FC63637}
		0xB1BADD73, 0xA606, 0x4CB7, { 0x83, 0x4C, 0x54, 0x17, 0x7F, 0xC6, 0x36, 0x37 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

BOOL CMarmaladePlugIn::OnLoadPlugIn()
{
	// Description:
	//   Called after the plug-in is loaded and the constructor has been
	//   run. This is a good place to perform any significant initialization,
	//   license checking, and so on.  This function must return TRUE for
	//   the plug-in to continue to load.

	m_pRdkPlugIn = new CMarmaladeRdkPlugIn;

	ON_wString wStr;

	if (!m_pRdkPlugIn->Initialize())
	{
		delete m_pRdkPlugIn;
		m_pRdkPlugIn = nullptr;

		wStr.Format(L"Failed to load %s, version %s.  RDK initialization failed\n", PlugInName(), PlugInVersion());
		RhinoApp().Print(wStr);

		return FALSE;
	}

	wStr.Format(L"Loading %s, version %s\n", PlugInName(), PlugInVersion());
	RhinoApp().Print(wStr);

//#ifdef _DEBUG
//	RhinoApp().Print(L"%s compiled with RDK_SDK_VERSION %s, ", PlugInName(), RDK_SDK_VERSION);
//	RhinoApp().Print(L"running on RDK version %s, %s\n", ::RhRdkVersion(), ::RhRdkBuildDate());
//#endif

	m_event_watcher.Register();
	m_event_watcher.Enable(TRUE);

	if (RhinoApp().GetDefaultRenderApp() == PlugInID())
	{
		AddMarmaladeMenu();
	}

	// This timer watches the content editor so that the menu is up to date.
	m_iTimer = RhinoApp().GetMainWnd()->SetTimer(iTimerId, 500, TimerProc);

	CRhinoDoc* pDoc = RhinoApp().ActiveDoc();
	if (nullptr != pDoc)
	{
		m_event_watcher.OnNewDocument(*pDoc);
	}

	return CRhinoRenderPlugIn::OnLoadPlugIn();
}

void CMarmaladePlugIn::OnUnloadPlugIn()
{
	// Description:
	//   Called when the plug-in is about to be unloaded.  After
	//   this function is called, the destructor will be called.

	delete m_pMenu;
	m_pMenu = nullptr;

	if (nullptr != m_pRdkPlugIn)
	{
		m_pRdkPlugIn->Uninitialize();

		delete m_pRdkPlugIn;
		m_pRdkPlugIn = nullptr;

		m_event_watcher.Enable(FALSE);
		m_event_watcher.UnRegister();
	}

	CRhinoRenderPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CMarmaladePlugIn::AddToPlugInHelpMenu() const
{
	// Description:
	//   Return true to have your plug-in name added to the Rhino help menu.
	//   OnDisplayPlugInHelp will be called when to activate your plug-in help.

	return TRUE;
}

BOOL CMarmaladePlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
	// Description:
	//   Called when the user requests help about your plug-in.
	//   It should display a standard Windows Help file (.hlp or .chm).

	// TODO: Add support for online help here.
	return CRhinoRenderPlugIn::OnDisplayPlugInHelp(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// Render overrides

CRhinoCommand::result CMarmaladePlugIn::Render(const CRhinoCommandContext& context, bool bPreview)
{
	// Description:
	//   Called by the Render and RenderPreview commands if this application is both
	//   a Render plug-in and is set as the default render engine.
	// Parameters:
	//   context [in] Command parameters passed to the render command.
	//   bPreview [in] If true, a faster, lower quality rendering is expected.

	const auto* pDoc = context.Document();
	if (nullptr == pDoc)
		return CRhinoCommand::failure;

	CMarmaladeSdkRender render(context, *this, L"Marmalade", IDI_MARMALADE, bPreview);

	const ON_2iSize size = render.RenderSize(*pDoc, true);
	if (CRhinoSdkRender::render_ok == render.Render(size))
		return CRhinoCommand::success;

	return CRhinoCommand::failure;
}

CRhinoCommand::result CMarmaladePlugIn::RenderWindow(const CRhinoCommandContext& context, bool bPreview,
                                                     CRhinoView* pView, const LPRECT rect, bool bInWindow, bool bBlowUp)
{
	// Description:
	//   Called by Render command if this application is both
	//   a Render plug-in and is set as the default render engine.
	// Parameters:
	//   context [in] Command paramaters passed to the render command.
	//   bPreview [in] If true, a faster, lower quality rendering is expected.
	//   view [in] View to render.
	//   rect [in] Rectangle to render in viewport window coordinates.

	CMarmaladeSdkRender render(context, *this, L"Marmalade", IDI_MARMALADE, bPreview);

	if (CRhinoSdkRender::render_ok == render.RenderWindow(pView, rect, bInWindow))
		return CRhinoCommand::success;

	return CRhinoCommand::failure;
}

// Render methods

bool CMarmaladePlugIn::SceneChanged(void) const
{
	return m_event_watcher.RenderSceneModified();
}

bool CMarmaladePlugIn::LightingChanged(void) const
{
	return m_event_watcher.RenderLightingModified();
}

void CMarmaladePlugIn::SetSceneChanged(bool bChanged)
{
	m_event_watcher.SetRenderMeshFlags(bChanged);
	m_event_watcher.SetMaterialFlags(bChanged);
}

void CMarmaladePlugIn::SetLightingChanged(bool bChanged)
{
	m_event_watcher.SetLightFlags(bChanged);
}

#define MATERIAL_EDITOR_CMD     L"MaterialEditor"
#define ENVIRONMENT_EDITOR_CMD  L"EnvironmentEditor"
#define TEXTURE_EDITOR_CMD      L"TexturePalette"

enum
{
	ID_MATERIAL_EDITOR_CMD = 1700,
	ID_ENVIRONMENT_EDITOR_CMD,
	ID_TEXTURE_EDITOR_CMD,
};

void CMarmaladePlugIn::AddMarmaladeMenu(void)
{
	if (nullptr == m_pMenu)
	{
		m_pMenu = new CMenu;
		m_pMenu->CreateMenu();

		m_pMenu->AppendMenu(MF_STRING, ID_MATERIAL_EDITOR_CMD,   _T("&Material Editor"));
		m_pMenu->AppendMenu(MF_STRING, ID_ENVIRONMENT_EDITOR_CMD,_T("&Environment Editor"));
		m_pMenu->AppendMenu(MF_STRING, ID_TEXTURE_EDITOR_CMD,    _T("&Texture Editor"));
		m_pMenu->AppendMenu(MF_SEPARATOR);
//		m_pMenu->AppendMenu(MF_STRING, ID_CONTENT_BROWSER_CMD,   _T("&Content Browser")); // Obsolete.
//		m_pMenu->AppendMenu(MF_SEPARATOR);
//		m_pMenu->AppendMenu(MF_STRING, ID_RENDERER_SETTINGS_CMD, _T("&Renderer Settings")); // Now uses custom render settings sections.

		InsertPlugInMenuToRhinoMenu(*m_pMenu, _T("&Marmalade"));
	}
}

void CMarmaladePlugIn::RemoveMarmaladeMenu(void)
{
	if (nullptr != m_pMenu)
	{
		RemovePlugInMenuFromRhino(m_pMenu->GetSafeHmenu());
		delete m_pMenu;
		m_pMenu = nullptr;
	}
}

BOOL CMarmaladePlugIn::OnPlugInMenuCommand(WPARAM wParam)
{
	const wchar_t* wsz= nullptr;

	switch (LOWORD(wParam))
	{
	case ID_MATERIAL_EDITOR_CMD:
		wsz = MATERIAL_EDITOR_CMD;
		break;

	case ID_ENVIRONMENT_EDITOR_CMD:
		wsz = ENVIRONMENT_EDITOR_CMD;
		break;

	case ID_TEXTURE_EDITOR_CMD:
		wsz = TEXTURE_EDITOR_CMD;
		break;
	}

	const unsigned int rhino_doc_sn = CRhinoDoc::ModelessUserInterfaceDocSerialNumber();
	RhinoApp().RunScript(rhino_doc_sn, wsz, 0);

	return TRUE;
}

void CMarmaladePlugIn::UpdateMarmaladeMenuState(void)
{
	if (nullptr == m_pMenu)
		return;

	auto rhino_doc_sn = CRhinoDoc::ModelessUserInterfaceDocSerialNumber();
	auto* pDoc = CRhinoDoc::FromRuntimeSerialNumber(rhino_doc_sn);
	if (nullptr == pDoc)
		return;

	bool bVis = ::RhRdkIsThumbnailEditorVisible(*pDoc, CRhRdkContent::Kinds::Material);
	m_pMenu->CheckMenuItem(ID_MATERIAL_EDITOR_CMD, (bVis ? MF_CHECKED : MF_UNCHECKED));

	bVis = ::RhRdkIsThumbnailEditorVisible(*pDoc, CRhRdkContent::Kinds::Environment);
	m_pMenu->CheckMenuItem(ID_ENVIRONMENT_EDITOR_CMD, (bVis ? MF_CHECKED : MF_UNCHECKED));

	bVis = ::RhRdkIsThumbnailEditorVisible(*pDoc, CRhRdkContent::Kinds::Texture);
	m_pMenu->CheckMenuItem(ID_TEXTURE_EDITOR_CMD, (bVis ? MF_CHECKED : MF_UNCHECKED));
}

bool CMarmaladePlugIn::IsMarmaladeCurrentRenderer(void) const
{
	if (RhinoApp().GetDefaultRenderApp() == PlugInID())
		return true;

	return false;
}

void CMarmaladePlugIn::AddPagesToObjectPropertiesDialog(CRhinoPropertiesPanelPageCollection& coll)
{
	// This method is called once at the completion of loading the plug-in.

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	coll.Add(new CMarmaladeViewPropertiesPage);
}
