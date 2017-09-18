// DocumentPropertiesPage.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "DocumentPropertiesPage.h"

// CDocumentPropertiesPage dialog

IMPLEMENT_DYNAMIC(CDocumentPropertiesPage, __base_class)

CDocumentPropertiesPage::CDocumentPropertiesPage()
: TRhinoOptionsPage<CDialog>(CDocumentPropertiesPage::IDD, 0, true)
{

}

CDocumentPropertiesPage::~CDocumentPropertiesPage()
{
}

void CDocumentPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
  __base_class::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON1, m_color_button);
}


BEGIN_MESSAGE_MAP(CDocumentPropertiesPage, __base_class)
END_MESSAGE_MAP()


// CDocumentPropertiesPage message handlers

const wchar_t* CDocumentPropertiesPage::EnglishTitle() const { return L"Test Page"; }
const wchar_t* CDocumentPropertiesPage::LocalTitle() const { return EnglishTitle(); }
CRhinoCommand::result CDocumentPropertiesPage::RunScript(CRhinoOptionsPageEventArgs&) { return CRhinoCommand::success; }

void CDocumentPropertiesPage::UpdatePage(CRhinoOptionsPageEventArgs& e)
{
  // Initialize the page when activated
  m_color_button.SetColor(CApp::App().ObjectColor(e.Document()));
}

bool CDocumentPropertiesPage::Apply(CRhinoOptionsPageEventArgs& e)
{
  // Set the document specific object color
  CApp::App().SetObjectColor(e.Document(), m_color_button.Color());
  return true;
}

void CDocumentPropertiesPage::OnRestoreDefaultsClick(CRhinoOptionsPageEventArgs&)
{
  // Set the color button to the default application object color
  m_color_button.SetColor(CApp::DEFULLT_OBJECT_COLOR);
}
