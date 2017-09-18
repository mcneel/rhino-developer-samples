// PropertiesPage.cpp : implementation file
//

#include "stdafx.h"
#include "PropertiesPage.h"
#include "App.h"

class CPropertiesPageCommand : public CRhinoTestCommand
{
public:
  CPropertiesPageCommand()
  : m_by_layer(false)
  , m_page(nullptr)
  {
  }

  ~CPropertiesPageCommand()
  {
  }

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {E547CD29-920F-4EF9-92EC-3F4AE9D9E619}
    static const GUID command_id =
    { 0xe547cd29, 0x920f, 0x4ef9, { 0x92, 0xec, 0x3f, 0x4a, 0xe9, 0xd9, 0xe6, 0x19 } };
    return command_id;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"TestPropertiesPageModifyObjects"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"TestPropertiesPageModifyObjects"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

  bool m_by_layer;
  ON_SimpleArray<ON_UUID>m_object_ids;
  CPropertiesPage* m_page;

  CRhinoCommand::result MakeByLayer(ON_SimpleArray<const CRhinoObject*>& objectList);
  CRhinoCommand::result MakeObjectColor(ON_SimpleArray<const CRhinoObject*>& objectList);
};

// The one and only CPropertiesPageCommand object.  
// Do NOT create any other instance of a CPropertiesPageCommand class.
static class CPropertiesPageCommand thePropertiesPageCommand;

CRhinoCommand::result CPropertiesPageCommand::RunCommand( const CRhinoCommandContext& context )
{
  if (m_page == nullptr)
    return CRhinoCommand::failure;
  int modified = 0;
  for (int i = 0, count = m_object_ids.Count(); i < count; i++)
  {
    const CRhinoObject* object = context.m_doc.LookupObject(m_object_ids[i]);
    if (object && !object->IsDeleted())
    {
        modified += m_by_layer
        ? m_page->ToByLayer(object) 
        : m_page->ToObjectColor(object);
    }
  }
  m_object_ids.Empty();
  m_page->ModifiedMessage(m_by_layer, modified);
  m_page = nullptr;
  return modified > 0 ? CRhinoCommand::success : CRhinoCommand::nothing;
}

// CPropertiesPage dialog

IMPLEMENT_DYNAMIC(CPropertiesPage, CRhinoObjectPropertiesDialogPageEx)

CPropertiesPage::CPropertiesPage()
	: CRhinoObjectPropertiesDialogPageEx(CPropertiesPage::IDD, NULL)
  , m_icon(nullptr)
{
}

CPropertiesPage::~CPropertiesPage()
{
}

void CPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
  CRhinoObjectPropertiesDialogPageEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_MESSAGE_TEXT, m_message_text);
  DDX_Control(pDX, IDC_STATIC_COLOR_SWATCH, m_color_swatch);
}


BEGIN_MESSAGE_MAP(CPropertiesPage, CRhinoObjectPropertiesDialogPageEx)
  ON_BN_CLICKED(IDC_BUTTON1, &CPropertiesPage::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CPropertiesPage::OnBnClickedButton2)
  ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CPropertiesPage::OnInitDialog()
{
  BOOL result = CRhinoObjectPropertiesDialogPageEx::OnInitDialog();
  return result;
}

void CPropertiesPage::OnSize(UINT nType, int cx, int cy)
{
  CRhinoObjectPropertiesDialogPageEx::OnSize(nType, cx, cy);

  if (cx < 10)
    return;

  CRect client_r;
  GetClientRect(client_r);

  UINT ids[] = { IDC_STATIC, IDC_STATIC_COLOR_SWATCH, IDC_MESSAGE_TEXT, IDC_BUTTON1, IDC_BUTTON2};
  const int margin = 2;
  const int left = client_r.left + margin;
  const int right = client_r.right - margin;
  
  for (int i = 0, count = _countof(ids); i < count; i++)
  {
    CWnd* control = GetDlgItem(ids[i]);
    if (control == NULL)
      continue;
    CRect r;
    control->GetWindowRect(r);
    ScreenToClient(r);
    r.left = left;
    r.right = right;
    control->MoveWindow(r);
  }
}

// CPropertiesPage message handlers

const wchar_t* CPropertiesPage::EnglishPageTitle() { return L"Test Properties Page"; }
const wchar_t* CPropertiesPage::LocalPageTitle() { return EnglishPageTitle(); }
HICON CPropertiesPage::Icon(void) const
{
  if (m_icon == nullptr)
    m_icon = ::LoadIcon(m_pModuleState->m_hCurrentResourceHandle, MAKEINTRESOURCE(IDI_PROPERTIES_PAGE));
  return m_icon;
}

void CPropertiesPage::InitControls( const CRhinoObject* new_obj)
{
  int our_color_objects = 0;
  CRhinoDoc* doc = nullptr;
  for (int i = 0, count = SelectedObjectCount(); i < count; i++)
  {
    const CRhinoObject* object = GetSelectedObject(i);
    if (object && doc == NULL)
      doc = object->Document();
    if (IncludeObject(object) && IsOurColor(object))
      our_color_objects++;
  }
  ON_wString message;
  message.Format(L"%d Colored objects selected", our_color_objects);
  m_message_text.SetWindowText(message);
  if (doc)
    m_color_swatch.SetColor(CApp::App().ObjectColor(doc));
  else
    m_color_swatch.SetColor(::GetSysColor(COLOR_BTNFACE));
}

bool CPropertiesPage::IncludeObject(const CRhinoObject* object) const
{
  return object && object->Document() && object->IsMeshable(ON::render_mesh);
}

bool CPropertiesPage::IsOurColor(const CRhinoObject* object) const
{
  if (object == nullptr)
    return false;
  const CRhinoObjectAttributes& attribs = object->Attributes();
  return attribs.ColorSource() == ON::color_from_object && attribs.m_color == CApp::App().ObjectColor(object->Document());
}

BOOL CPropertiesPage::AddPageToControlBar( const CRhinoObject* obj) const
{
  // Only care if one or more meshable object is selected
  for (int i = 0, count = SelectedObjectCount(); i < count; i++)
    if (IncludeObject(GetSelectedObject(i)))
      return true;
  return false;
}

CRhinoObjectPropertiesDialogPageEx::page_type CPropertiesPage::PageType() const { return CRhinoObjectPropertiesDialogPageEx::custom_page; }

CRhinoCommand::result CPropertiesPage::RunScript( ON_SimpleArray<const CRhinoObject*>& objects){ return CRhinoCommand::success; }

void CPropertiesPage::RunModifyCommand(bool byLayer)
{
  thePropertiesPageCommand.m_object_ids.Empty();
  for (int i = 0, count = SelectedObjectCount(); i < count; i++)
  {
    const CRhinoObject* object = GetSelectedObject(i);
    if (!IncludeObject(object))
      continue;
    if (byLayer && object->Attributes().ColorSource() == ON::color_from_layer)
      continue;
    if (!byLayer && IsOurColor(object))
      continue;
    thePropertiesPageCommand.m_object_ids.Append(object->Attributes().m_uuid);
  }

  if (thePropertiesPageCommand.m_object_ids.Count() < 1)
  {
    ::RhinoMessageBox(RhinoApp().MainWnd(), L"Nothing modified!", LocalPageTitle(), MB_OK);
    return;
  }

  thePropertiesPageCommand.m_page = this;
  thePropertiesPageCommand.m_by_layer = byLayer;
  ON_wString macro;
  macro.Format(L"_noecho !_-%s", thePropertiesPageCommand.EnglishCommandName());
  RhinoApp().RunScript(macro);
}

void CPropertiesPage::OnBnClickedButton1()
{
  RunModifyCommand(false);
}

void CPropertiesPage::OnBnClickedButton2()
{
  RunModifyCommand(true);
}

int CPropertiesPage::ToByLayer(const CRhinoObject* object) const
{
  if (!IncludeObject(object))
    return 0;
  ON_3dmObjectAttributes attribs(object->Attributes());
  if (attribs.ColorSource() == ON::color_from_layer)
    return 0;
  attribs.SetColorSource(ON::color_from_layer);
  return object->Document()->ModifyObjectAttributes(CRhinoObjRef(object), attribs) ? 1 : 0;
}

int CPropertiesPage::ToObjectColor(const CRhinoObject* object) const
{
  if (!IncludeObject(object) || IsOurColor(object))
    return 0;
  ON_3dmObjectAttributes attribs(object->Attributes());
  attribs.SetColorSource(ON::color_from_object);
  attribs.m_color = CApp::App().ObjectColor(object->Document());
  return object->Document()->ModifyObjectAttributes(CRhinoObjRef(object), attribs) ? 1 : 0;
}

void CPropertiesPage::ModifiedMessage(bool byLayer, int modifiedCount) const
{
  ON_wString message;
  if (modifiedCount < 1)
    message = L"Nothing was modified";
  else if (byLayer)
    message.Format(L"%d Objects were modified and are color by layer", modifiedCount);
  else
    message.Format(L"%d Objects colors were changed", modifiedCount);
  ::RhinoMessageBox(RhinoApp().MainWnd(), message, L"Test Object Properties Page", MB_OK);
}
