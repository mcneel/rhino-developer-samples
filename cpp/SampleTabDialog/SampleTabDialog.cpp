#include "stdafx.h"
#include "SampleTabDialog.h"

// CSampleTabDialog dialog

IMPLEMENT_DYNAMIC(CSampleTabDialog, CRhinoUiDialog)

CSampleTabDialog::CSampleTabDialog(CWnd* pParent /*=NULL*/)
	: CRhinoUiDialog(CSampleTabDialog::IDD, pParent)
{
}

CSampleTabDialog::~CSampleTabDialog()
{
}

void CSampleTabDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoUiDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TAB, m_tab_ctrl);
}


BEGIN_MESSAGE_MAP(CSampleTabDialog, CRhinoUiDialog)
END_MESSAGE_MAP()

// CSampleTabDialog message handlers

BOOL CSampleTabDialog::OnInitDialog()
{
  CRhinoUiDialog::OnInitDialog();

  m_tab_ctrl.AddPage(m_page1, L"Page 1");
  m_tab_ctrl.AddPage(m_page2, L"Page 2");
  m_tab_ctrl.AddPage(m_page3, L"Page 3");

  return TRUE;
}
