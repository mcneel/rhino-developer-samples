#include "stdafx.h"
#include "SampleObjectPropertiesPageDialog.h"

CSampleObjectPropertiesPageDialog::CSampleObjectPropertiesPageDialog()
  : TRhinoPropertiesPanelPage<CRhinoDialog>(CSampleObjectPropertiesPageDialog::IDD, IDI_OBJPROPS_ICON, true)
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

const wchar_t* CSampleObjectPropertiesPageDialog::Varies() const
{
  return L"varies";
}

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
  m_old_value.Empty();
  m_new_value.Empty();
  bool bVaries = false;

  const int count = args.ObjectCount();
  if (0 == count)
  {
    m_edit.SetWindowText(ON_wString::EmptyString);
    m_button.EnableWindow(FALSE);
    return;
  }

  ON_wString value;
  const CRhinoObject* obj = args.ObjectAt(0);
  if (nullptr != obj)
  {
    value = obj->Attributes().m_url;
    value.TrimLeftAndRight();
  }

  for (int i = 1; i < count; i++)
  {
    obj = args.ObjectAt(i);
    if (nullptr != obj)
    {
      ON_wString str = obj->Attributes().m_url;
      str.TrimLeftAndRight();
      if (0 != value.CompareOrdinal(str, true))
      {
        bVaries = true;
        break;
      }
    }
  }

  if (bVaries)
  {
    m_edit.SetWindowText(Varies());
    m_button.EnableWindow(FALSE);
  }
  else if (value.IsEmpty())
  {
    m_edit.SetWindowText(ON_wString::EmptyString);
    m_button.EnableWindow(FALSE);
  }
  else
  {
    m_old_value = value;
    m_edit.SetWindowText(m_old_value);
    m_button.EnableWindow(TRUE);
  }
  m_edit.EmptyUndoBuffer();
}

void CSampleObjectPropertiesPageDialog::OnModifyPage(IRhinoPropertiesPanelPageEventArgs& args)
{
  // Check undo handling
  CRhinoDoc* doc = args.Document();
  if (nullptr == doc || 0 == args.ObjectCount())
    return;
  
  unsigned int undo_record = doc->BeginUndoRecordEx(L"Hyperlink change");

  for (int i = 0; i < args.ObjectCount(); i++)
  {
    const CRhinoObject* obj = args.ObjectAt(i);
    if (nullptr != obj)
    {
      ON_3dmObjectAttributes attrib = obj->Attributes();
      attrib.m_url = m_new_value;
      doc->ModifyObjectAttributes(CRhinoObjRef(obj), attrib, false);
    }
  }

  if (undo_record)
    doc->EndUndoRecord(undo_record);

  m_old_value = m_new_value;
  m_new_value.Empty();
  m_edit.EmptyUndoBuffer();
}

CRhinoCommand::result CSampleObjectPropertiesPageDialog::RunScript(IRhinoPropertiesPanelPageEventArgs& args)
{
  // Scriptable version of our dialog box is handled here.
  const int count = args.ObjectCount();
  if (0 == count)
    return CRhinoCommand::nothing;

  UpdatePage(args);

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Hyperlink");
  gs.SetDefaultString(m_old_value.IsEmpty() ? Varies() : m_old_value);
  gs.AcceptNothing();
  CRhinoGet::result res = gs.GetString();
  if (res == CRhinoGet::cancel)
    return CRhinoCommand::cancel;
  else if (res == CRhinoGet::nothing)
    return CRhinoCommand::nothing;
  else if (res != CRhinoGet::string)
    return CRhinoCommand::failure;

  ON_wString value = gs.String();
  value.TrimLeftAndRight();
  if (!value.IsEmpty() && 0 != value.CompareOrdinal(Varies(), false))
  {
    if (0 != m_old_value.CompareOrdinal(value, false))
    {
      m_new_value = value;
      IRhinoPropertiesPanelPageHost* host = PropertiesPanelPageHost();
      if (nullptr != host)
        host->ModifyPage();
    }
  }

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
  CString str;
  m_edit.GetWindowText(str);

  ON_wString value = str;
  value.TrimLeftAndRight();

  BOOL bEnable = FALSE;
  if (!value.IsEmpty() && 0 != value.CompareOrdinal(Varies(), false))
    bEnable = TRUE;

  m_button.EnableWindow(bEnable);
}

void CSampleObjectPropertiesPageDialog::OnButton()
{
  CString str;
  m_edit.GetWindowText(str);

  ON_wString value = str;
  value.TrimLeftAndRight();

  if (!value.IsEmpty() && 0 != value.CompareOrdinal(Varies(), false))
    ::ShellExecute(RhinoApp().MainWnd(), L"open", value, nullptr, nullptr, SW_SHOWNORMAL);
}

void CSampleObjectPropertiesPageDialog::OnEditKillFocus()
{
  // Check if anything has changed
  if (!m_edit.CanUndo())
    return;

  CString str;
  m_edit.GetWindowText(str);

  ON_wString value = str;
  value.TrimLeftAndRight();

  if (!value.IsEmpty() && 0 != value.CompareOrdinal(Varies(), false))
  {
    if (0 != m_old_value.CompareOrdinal(value, false))
    {
      m_new_value = value;
      IRhinoPropertiesPanelPageHost* host = PropertiesPanelPageHost();
      if (nullptr != host)
        host->ModifyPage();
    }
  }
}
