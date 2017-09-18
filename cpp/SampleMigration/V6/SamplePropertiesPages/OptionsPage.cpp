// OptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "OptionsPage.h"
#include "App.h"

// COptionsPage dialog

IMPLEMENT_DYNAMIC(COptionsPage, __base_class)

COptionsPage::COptionsPage()
: TRhinoOptionsPage<CDialog>(COptionsPage::IDD, 0, true)
{

}

COptionsPage::~COptionsPage()
{
}

void COptionsPage::DoDataExchange(CDataExchange* pDX)
{
  __base_class::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON2, m_obj_color_button);
}


BEGIN_MESSAGE_MAP(COptionsPage, __base_class)
END_MESSAGE_MAP()


// COptionsPage message handlers

const wchar_t* COptionsPage::EnglishTitle() const { return L"Test Page"; }
const wchar_t* COptionsPage::LocalTitle() const { return EnglishTitle(); }
CRhinoCommand::result COptionsPage::RunScript(CRhinoOptionsPageEventArgs&) { return CRhinoCommand::success; }

void COptionsPage::UpdatePage(CRhinoOptionsPageEventArgs& e)
{
  // Initialize the page when activated
  m_obj_color_button.SetColor(CApp::App().DefaultObjectColor());
}

bool COptionsPage::Apply(CRhinoOptionsPageEventArgs&)
{
  // Apply the application color setting
  CApp::App().SetDefaultObjectColor(m_obj_color_button.Color());
  return true;
}

void COptionsPage::OnRestoreDefaultsClick(CRhinoOptionsPageEventArgs&)
{
  // Set the color button to the default application object color
  m_obj_color_button.SetColor(CApp::DEFULLT_OBJECT_COLOR);
}
