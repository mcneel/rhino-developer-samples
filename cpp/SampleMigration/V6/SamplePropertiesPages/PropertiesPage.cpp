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
    static const GUID V5PageTestCommand_UUID =
    { 0xe547cd29, 0x920f, 0x4ef9,{ 0x92, 0xec, 0x3f, 0x4a, 0xe9, 0xd9, 0xe6, 0x19 } };
    return V5PageTestCommand_UUID;
  }

  // Returns the English command name.
  const wchar_t* EnglishCommandName() { return L"TestPropertiesPageModifyObjects"; }

  // Returns the localized command name.
  const wchar_t* LocalCommandName() const { return L"TestPropertiesPageModifyObjects"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

  bool m_by_layer;
  ON_SimpleArray<ON_UUID>m_object_ids;
  CPropertiesPage* m_page;

  CRhinoCommand::result MakeByLayer(ON_SimpleArray<const CRhinoObject*>& objectList);
  CRhinoCommand::result MakeObjectColor(ON_SimpleArray<const CRhinoObject*>& objectList);
};

// The one and only CCommandV5PageTest object.  
// Do NOT create any other instance of a CCommandV5PageTest class.
static class CPropertiesPageCommand thePropertiesPageCommand;

CRhinoCommand::result CPropertiesPageCommand::RunCommand(const CRhinoCommandContext& context)
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

IMPLEMENT_DYNAMIC(CPropertiesPage, __base_class)

CPropertiesPage::CPropertiesPage()
: TRhinoPropertiesPanelPage<CDialog>(CPropertiesPage::IDD, IDI_PROPERTIES_PAGE, false)
{
}

CPropertiesPage::~CPropertiesPage()
{
}

void CPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
  __base_class::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_MESSAGE_TEXT, m_message_text);
  DDX_Control(pDX, IDC_STATIC_COLOR_SWATCH, m_color_swatch);
}


BEGIN_MESSAGE_MAP(CPropertiesPage, __base_class)
  ON_BN_CLICKED(IDC_BUTTON1, &CPropertiesPage::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CPropertiesPage::OnBnClickedButton2)
  ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CPropertiesPage::OnInitDialog()
{
  BOOL result = __base_class::OnInitDialog();
  return result;
}

void CPropertiesPage::OnSize(UINT nType, int cx, int cy)
{
  __base_class::OnSize(nType, cx, cy);

  if (cx < 10)
    return;

  CRect client_r;
  GetClientRect(client_r);

  const int ids[] = { IDC_STATIC, IDC_STATIC_COLOR_SWATCH, IDC_MESSAGE_TEXT, IDC_BUTTON1, IDC_BUTTON2 };
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

const wchar_t* CPropertiesPage::EnglishTitle() const { return L"Test Properties Page"; }
const wchar_t* CPropertiesPage::LocalTitle() const { return EnglishTitle(); }

// This is now handled by TRhinoPropertiesPanelPage
//HICON CPropertiesPage::Icon(void) const
//{
//  if (m_icon == nullptr)
//    m_icon = ::LoadIcon(m_pModuleState->m_hCurrentResourceHandle, MAKEINTRESOURCE(IDI_PROPERTIES_PAGE));
//  return m_icon;
//}

void CPropertiesPage::UpdatePage(IRhinoPropertiesPanelPageEventArgs& e)
{
  int our_color_objects = 0;
  CRhinoDoc* doc = nullptr;
  for (int i = 0, count = e.ObjectCount(); i < count; i++)
  {
    const CRhinoObject* object = e.ObjectAt(i);
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

bool CPropertiesPage::IncludeInNavigationControl(IRhinoPropertiesPanelPageEventArgs& e) const
{
  // Only care if one or more mesh-able object is selected
  for (int i = 0, count = e.ObjectCount(); i < count; i++)
    if (IncludeObject(e.ObjectAt(i)))
      return true;
  return false;
}

RhinoPropertiesPanelPageType CPropertiesPage::PageType() const
{
  return RhinoPropertiesPanelPageType::Custom;
}

CRhinoCommand::result CPropertiesPage::RunScript(IRhinoPropertiesPanelPageEventArgs& e)
{
  return CRhinoCommand::success;
}

void CPropertiesPage::RunModifyCommand(bool byLayer)
{
  thePropertiesPageCommand.m_by_layer = byLayer;
  PropertiesPanelPageHost()->ModifyPage();
}

void CPropertiesPage::OnModifyPage(IRhinoPropertiesPanelPageEventArgs& args)
{
  thePropertiesPageCommand.m_object_ids.Empty();
  for (int i = 0, count = args.ObjectCount(); i < count; i++)
  {
    const CRhinoObject* object = args.ObjectAt(i);
    if (!IncludeObject(object))
      continue;
    if (thePropertiesPageCommand.m_by_layer && object->Attributes().ColorSource() == ON::color_from_layer)
      continue;
    if (!thePropertiesPageCommand.m_by_layer && IsOurColor(object))
      continue;
    thePropertiesPageCommand.m_object_ids.Append(object->Attributes().m_uuid);
  }

  if (thePropertiesPageCommand.m_object_ids.Count() < 1)
  {
    ::RhinoMessageBox(RhinoApp().MainWnd(), L"Nothing modified!", LocalTitle(), MB_OK);
    return;
  }

  thePropertiesPageCommand.m_page = this;
  ON_wString macro;
  macro.Format(L"_noecho !_-%s", thePropertiesPageCommand.EnglishCommandName());
  RhinoApp().RunScript(args.DocumentRuntimeSerialNumber(), macro);
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
