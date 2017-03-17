#include "stdafx.h"
#include "SampleOptionsListCtrlDlg.h"

CSampleOptionsListCtrl::CSampleOptionsListCtrl()
{}

CSampleOptionsListCtrl::~CSampleOptionsListCtrl()
{
}

void CSampleOptionsListCtrl::OnEndEditItem(CRhinoUiOptionsListCtrlItem& item)
{
  CRhinoUiOptionsListCtrl::OnEndEditItem(item);
}

bool CSampleOptionsListCtrl::OnItemButtonClicked(CRhinoUiOptionsListCtrlPushButton& item)
{
  CRhinoUiOptionsListCtrl::OnItemButtonClicked(item);
  return true;
}

bool CSampleOptionsListCtrl::OnItemButtonClicked(CRhinoUiOptionsListCtrlCheckBox& item)
{
  CRhinoUiOptionsListCtrl::OnItemButtonClicked(item);

  CSampleOptionsListCtrlDlg* pDlg = static_cast<CSampleOptionsListCtrlDlg*>(GetParent());
  if (0 == pDlg)
    return false;

  return true;
}

bool CSampleOptionsListCtrl::OnComboBoxPopulated(CRhinoUiOptionsListCtrlComboBox& item)
{
  CRhinoUiOptionsListCtrl::OnComboBoxPopulated(item);

  int cnt = item.GetCount();
  if (cnt < 1)
    return false; // Nothing added
  // Current item text
  CString sText = item.Text();
  // place holder for new text
  CString s;
  // current selection index
  int iCurSel = -1;
  // process combo box for string that matches sText
  for (int i = 0; i < cnt; i++)
  {
    item.GetLBText(i, s);
    if (0 == sText.CompareNoCase(s))
      iCurSel = i;
  }
  // Set current selection to matching string
  item.SetCurSel(iCurSel);
  return(cnt > 0);
}

bool CSampleOptionsListCtrl::PopulateItemComboBox(CRhinoUiOptionsListCtrlComboBox& item)
{
  CRhinoUiOptionsListCtrl::PopulateItemComboBox(item);

  CSampleOptionsListCtrlDlg* pDlg = static_cast<CSampleOptionsListCtrlDlg*>(GetParent());
  if (0 == pDlg)
    return false;

  return true;
}

void CSampleOptionsListCtrl::OnItemComboBoxSelChanged(CRhinoUiOptionsListCtrlComboBox& item)
{
  CRhinoUiOptionsListCtrl::OnItemComboBoxSelChanged(item);
  int iCurSel = item.GetCurSel();
  if (iCurSel < 0)
    return;
  // TODO: the combo box changed, update whatever it is connected to.
}

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSampleOptionsListCtrlDlg, CRhinoDialog)

CSampleOptionsListCtrlDlg::CSampleOptionsListCtrlDlg()
  : CRhinoDialog(CSampleOptionsListCtrlDlg::IDD)
{
}

CSampleOptionsListCtrlDlg::~CSampleOptionsListCtrlDlg()
{
}

void CSampleOptionsListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleOptionsListCtrlDlg, CRhinoDialog)
END_MESSAGE_MAP()

BOOL CSampleOptionsListCtrlDlg::OnInitDialog()
{
  AddResizeControl(TRUE);
  SetMinSizeToCurrentSize();
  EnableSaveDialogPositionAndSize(TRUE);

  m_Resize.Add(IDOK, CRhinoUiDialogItemResizer::resize_lockbottomleft);
  m_Resize.Add(IDCANCEL, CRhinoUiDialogItemResizer::resize_lockbottomleft);

  CWnd* pWnd = GetDlgItem(IDC_FRAME);
  ASSERT(pWnd);

  CRect rect;
  pWnd->GetWindowRect(rect);
  ScreenToClient(rect);

  DWORD dwStyle = pWnd->GetStyle();

  pWnd->DestroyWindow();
  pWnd = 0;

  m_options_list.SetBorders(0, 0, 0, 0);

  if (m_options_list.CreateOptionsList(this, dwStyle, rect))
  {
    m_options_list.SetIndentItems(true);

    m_options_list.m_RadioColumn.SetLabel(L"Radio button test");
    m_options_list.m_RadioColumn.SetAutoDelete(false);
    m_options_list.m_RadioColumn.AddRadioButton(L"One");
    m_options_list.m_RadioColumn.AddRadioButton(L"Two");
    m_options_list.m_RadioColumn.AddRadioButton(L"Three");
    m_options_list.m_RadioColumn.SetSelectedButton(1);
    m_options_list.m_RadioColumn.SetIndentLevel(1);
    m_options_list.AddItem(&m_options_list.m_RadioColumn);

    // Need to adjust the dialog to fit its contents then save the initial size
    // for use by the scroll bar calculator later on

    CRect rectBefore;
    m_options_list.GetWindowRect(rectBefore);
    m_options_list.SizeToContent(true);

    CRect rectAfter;
    m_options_list.GetWindowRect(rectAfter);
    if (rectAfter.Height() != rectBefore.Height())
    {
      CRect rectWindow;
      GetWindowRect(rectWindow);
      int cy = rectWindow.Height() + (rectAfter.Height() - rectBefore.Height());
      SetWindowPos(NULL, 0, 0, rectWindow.Width(), cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
      m_sizeMin.cy = cy;
    }
    else
    {
      m_sizeMin.cy = rectAfter.Height();
    }

    //m_Resize.Add( &m_options_list, CRhinoUiDialogItemResizer::resize_lockall );
    m_Resize.Add(&m_options_list, CRhinoUiDialogItemResizer::resize_lockwidth);
  }

  CRhinoDialog::OnInitDialog();

  return TRUE;
}

LRESULT CSampleOptionsListCtrlDlg::OnRegisteredMessage(WPARAM wParam, LPARAM lParam)
{
  LPRhinoUiOptionsListCtrlNM pNM = (LPRhinoUiOptionsListCtrlNM)wParam;
  LRESULT* pResult = (LRESULT*)lParam;

  *pResult = false;

  CRhinoUiOptionsListCtrlItem* pItem = pNM->m_item;
  if (NULL == pItem)
    return 0L;

  // int iItemIndex = (int)pNM->m_NMHDR.idFrom;
  UINT nNotificationCode = pNM->m_NMHDR.code;

  switch (nNotificationCode)
  {
  case CRhinoUiOptionsListCtrl::nc_on_item_button_clicked:
  {
    CRhinoUiOptionsListCtrlPushButton* pItemButton = static_cast<CRhinoUiOptionsListCtrlPushButton*>(pItem);
    CString s, sLabel = pItemButton->Label();
    if (sLabel.IsEmpty())
      sLabel = L"Untitled";
    s.Format(L"Button %s clicked", sLabel);
    MessageBox(s);
  }
  break;

  case CRhinoUiOptionsListCtrl::nc_on_item_check_box_clicked:
    if (static_cast<CRhinoUiOptionsListCtrlCheckBox*>(pItem)->GetCheck() == BST_UNCHECKED)
      pItem->SetText(L"Unchecked");
    else
      pItem->SetText(L"Checked");
    break;

  case CRhinoUiOptionsListCtrl::nc_on_combo_box_sel_changed:
  {
    CRhinoUiOptionsListCtrlComboBox* pItemComboBox = static_cast<CRhinoUiOptionsListCtrlComboBox*>(pItem);
    if (::IsWindow(pItemComboBox->m_hWnd))
    {
      CString s;
      pItemComboBox->GetTextFromListIndex(pItemComboBox->GetCurSel(), s);
      pItemComboBox->SetText(s);
    }
  }
  break;

  case CRhinoUiOptionsListCtrl::nc_on_radio_button_checked:
  {
    const CRhinoUiOptionsListCtrlRadioButtonColumnBtn* pBtn = static_cast<CRhinoUiOptionsListCtrlRadioButtonColumn*>(pItem)->GetSelectedButton();
    CString s;
    if (pBtn)
      s = pBtn->ButtonLabel();
    pItem->SetText(s);
  }
  break;

  case CRhinoUiOptionsListCtrl::nc_on_end_edit_item:
  {
    CString sLabel = pItem->Label();
    if (!sLabel.IsEmpty())
    {
      CString s;
      s.Format(L"nc_on_end_edit_item Label(%s)", sLabel);
      //MessageBox( s);
    }
  }
  break;
  }

  return 0L;
}
