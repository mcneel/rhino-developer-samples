#include "stdafx.h"
#include "SampleDocumentPropertiesPage.h"

CSampleDocumentPropertiesPage::CSampleDocumentPropertiesPage()
  : TRhinoOptionsPage<CRhinoDialog>(CSampleDocumentPropertiesPage::IDD, 0, true)
{
}

void CSampleDocumentPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
  __base_class::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON1, m_button);
  DDX_Control(pDX, IDC_SEP1, m_sep1);
}

BEGIN_MESSAGE_MAP(CSampleDocumentPropertiesPage, __base_class)
  ON_BN_CLICKED(IDC_BUTTON1, &CSampleDocumentPropertiesPage::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CSampleDocumentPropertiesPage::OnInitDialog()
{
  m_Resize.Add(IDC_SEP1, CRhinoUiDialogItemResizer::resize_lockwidth);

  __base_class::OnInitDialog();

  return TRUE;
}

const wchar_t* CSampleDocumentPropertiesPage::EnglishTitle() const
{
  return L"Sample Properties";
}

const wchar_t* CSampleDocumentPropertiesPage::LocalTitle() const
{
  return EnglishTitle();
}

bool CSampleDocumentPropertiesPage::Apply(CRhinoOptionsPageEventArgs& /*e*/)
{
  return true;
}

void CSampleDocumentPropertiesPage::UpdatePage(CRhinoOptionsPageEventArgs& /*e*/)
{
}

CRhinoCommand::result CSampleDocumentPropertiesPage::RunScript(CRhinoOptionsPageEventArgs& /*e*/)
{
  return CRhinoCommand::success;
}

void CSampleDocumentPropertiesPage::OnBnClickedButton1()
{
  RhinoMessageBox(GetSafeHwnd(), L"Pick me button pressed", LocalTitle(), MB_OK);
}
