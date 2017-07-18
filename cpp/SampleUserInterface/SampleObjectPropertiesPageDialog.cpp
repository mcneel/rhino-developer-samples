#include "stdafx.h"
#include "SampleObjectPropertiesPageDialog.h"

CSampleObjectPropertiesPageDialog::CSampleObjectPropertiesPageDialog()
  : TRhinoPropertiesPanelPage<CRhinoDialog>(CSampleObjectPropertiesPageDialog::IDD, IDI_OBJPROPS_ICON, true)
  , m_bVaries(false)
  , m_bDirty(false)
{
}

void CSampleObjectPropertiesPageDialog::DoDataExchange(CDataExchange* pDX)
{
  __base_class::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_S_HYPERLINK, m_separator);
  DDX_Control(pDX, IDC_E_HYPERLINK, m_edit);
  DDX_Control(pDX, IDC_B_HYPERLINK, m_button);
}

BEGIN_MESSAGE_MAP(CSampleObjectPropertiesPageDialog, __base_class)
  ON_EN_CHANGE(IDC_E_HYPERLINK, OnEditChange)
  ON_EN_KILLFOCUS(IDC_E_HYPERLINK, OnEditKillFocus)
  ON_BN_CLICKED(IDC_B_HYPERLINK, OnButton)
END_MESSAGE_MAP()

const wchar_t* CSampleObjectPropertiesPageDialog::EnglishTitle() const
{
  return L"Sample Properties";
}

const wchar_t* CSampleObjectPropertiesPageDialog::LocalTitle() const
{
  return EnglishTitle();
}

bool CSampleObjectPropertiesPageDialog::IncludeInNavigationControl(IRhinoPropertiesPanelPageEventArgs& /*args*/) const
{
  return true;
}

void CSampleObjectPropertiesPageDialog::UpdatePage(IRhinoPropertiesPanelPageEventArgs& args)
{
  const int count = args.ObjectCount();
  if (0 == count)
  {
    m_edit.SetWindowText(L"");
    m_button.EnableWindow(FALSE);
    return;
  }

  m_bVaries = false;
  m_bDirty = false;

  ON_wString hyperlink;
  const CRhinoObject* obj = args.ObjectAt(0);
  if (nullptr != obj)
  {
    hyperlink = obj->Attributes().m_url;
    hyperlink.TrimLeftAndRight();
  }

  for (int i = 1; i < count; i++)
  {
    obj = args.ObjectAt(i);
    if (nullptr != obj)
    {
      ON_wString str = obj->Attributes().m_url;
      str.TrimLeftAndRight();
      if (0 != hyperlink.CompareOrdinal(str, true))
      {
        m_bVaries = true;
        break;
      }
    }
  }

  if (m_bVaries)
  {
    m_hyperlink = L"varies";
    m_edit.SetWindowText(m_hyperlink);
    m_button.EnableWindow(FALSE);
  }
  else if (hyperlink.IsEmpty())
  {
    m_hyperlink = L"";
    m_edit.SetWindowText(m_hyperlink);
    m_button.EnableWindow(FALSE);
  }
  else
  {
    m_hyperlink = hyperlink;
    m_edit.SetWindowText(m_hyperlink);
    m_button.EnableWindow(TRUE);
  }
}

void CSampleObjectPropertiesPageDialog::OnModifyPage(IRhinoPropertiesPanelPageEventArgs& args)
{
  // Check undo handling

}

CRhinoCommand::result CSampleObjectPropertiesPageDialog::RunScript(IRhinoPropertiesPanelPageEventArgs& args)
{
  // Scriptable version of our dialog box is handled here.
  const int count = args.ObjectCount();
  if (0 == count)
    return CRhinoCommand::nothing;

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Hyperlink");
  gs.SetDefaultString(m_hyperlink);
  gs.AcceptNothing();
  CRhinoGet::result res = gs.GetString();
  if (res == CRhinoGet::cancel)
    return CRhinoCommand::cancel;
  else if (res == CRhinoGet::nothing)
    return CRhinoCommand::nothing;
  else if (res != CRhinoGet::string)
    return CRhinoCommand::failure;

  ON_wString hyperlink = gs.String();
  hyperlink.TrimLeftAndRight();
  if (0 == m_hyperlink.CompareOrdinal(hyperlink, false))
    return CRhinoCommand::nothing;

  //DoModification(args, hyperlink);
  IRhinoPropertiesPanelPageHost* host = PropertiesPanelPageHost();
  if (nullptr != host)
    host->ModifyPage();

  return CRhinoCommand::success;
}

BOOL CSampleObjectPropertiesPageDialog::OnInitDialog()
{
  m_Resize.Add(IDC_S_HYPERLINK, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_E_HYPERLINK, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);

  __base_class::OnInitDialog();

  return TRUE;
}

void CSampleObjectPropertiesPageDialog::OnEditChange()
{
  CString text;
  m_edit.GetWindowText(text);

  ON_wString str = text;
  str.TrimLeftAndRight();

  if (str.IsEmpty())
    m_button.EnableWindow(FALSE);
  else if (0 == str.CompareOrdinal(L"varies", false))
    m_button.EnableWindow(FALSE);
  else
    m_button.EnableWindow(TRUE);
}

void CSampleObjectPropertiesPageDialog::OnButton()
{
  CString text;
  m_edit.GetWindowText(text);

  ON_wString str = text;
  str.TrimLeftAndRight();
  if (str.IsEmpty() || 0 == str.CompareOrdinal(L"varies", false))
    return;

  ::ShellExecute(RhinoApp().MainWnd(), L"open", str, NULL, NULL, SW_SHOWNORMAL);
}

void CSampleObjectPropertiesPageDialog::OnEditKillFocus()
{
  CString text;
  m_edit.GetWindowText(text);

  ON_wString str = text;
  str.TrimLeftAndRight();

  if (0 == m_hyperlink.CompareOrdinal(str, false))
    return;

  IRhinoPropertiesPanelPageHost* host = PropertiesPanelPageHost();
  if (nullptr != host)
    host->ModifyPage();
}

//void CSampleObjectPropertiesPageDialog::DoModification(const wchar_t* hyperlink)
//{
//  PropertiesPanelPageHost()->ModifyPage();
//  ModifyPa
//  CRhinoDoc* doc = args.Document();
//  if (nullptr == doc || 0 == args.ObjectCount())
//    return;
//
//  m_hyperlink = hyperlink;
//  m_bVaries = false;
//
//  for (int i = 0; i < args.ObjectCount(); i++)
//  {
//    const CRhinoObject* obj = args.ObjectAt(i);
//    if (nullptr != obj)
//    {
//      ON_3dmObjectAttributes attrib = obj->Attributes();
//      attrib.m_url = hyperlink;
//      doc->ModifyObjectAttributes(CRhinoObjRef(obj), attrib, false);
//    }
//  }
//}
