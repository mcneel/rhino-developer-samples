#include "stdafx.h"
#include "SampleOptionsPage.h"

CSampleOptionsPage::CSampleOptionsPage()
  : TRhinoOptionsPage<CRhinoDialog>(CSampleOptionsPage::IDD, 0, true)
{
}

void CSampleOptionsPage::DoDataExchange(CDataExchange* pDX)
{
  __base_class::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON1, m_button);
  DDX_Control(pDX, IDC_SEP1, m_sep1);
}

BEGIN_MESSAGE_MAP(CSampleOptionsPage, __base_class)
  ON_BN_CLICKED(IDC_BUTTON1, &CSampleOptionsPage::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CSampleOptionsPage::OnInitDialog()
{
  m_Resize.Add(IDC_SEP1, CRhinoUiDialogItemResizer::resize_lockwidth);

  __base_class::OnInitDialog();

  return TRUE;
}

const wchar_t* CSampleOptionsPage::EnglishTitle() const
{
  return L"Sample Options";
}

const wchar_t* CSampleOptionsPage::LocalTitle() const
{
  return EnglishTitle();
}

bool CSampleOptionsPage::Apply(CRhinoOptionsPageEventArgs& /*e*/)
{
  return true;
}

void CSampleOptionsPage::UpdatePage(CRhinoOptionsPageEventArgs& /*e*/)
{
}

CRhinoCommand::result CSampleOptionsPage::RunScript(CRhinoOptionsPageEventArgs& /*e*/)
{
  // TODO...
  return CRhinoCommand::success;
}

void CSampleOptionsPage::OnBnClickedButton1()
{
  RhinoMessageBox(GetSafeHwnd(), L"Pick me button pressed", LocalTitle(), MB_OK);
}
