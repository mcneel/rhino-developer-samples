// OptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "OptionsPage.h"
#include "App.h"

// COptionsPage dialog

IMPLEMENT_DYNAMIC(COptionsPage, CRhinoOptionsDialogPage)

COptionsPage::COptionsPage()
: CRhinoOptionsDialogPage(COptionsPage::IDD)
{

}

COptionsPage::~COptionsPage()
{
}

void COptionsPage::DoDataExchange(CDataExchange* pDX)
{
  CRhinoOptionsDialogPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON2, m_obj_color_button);
}


BEGIN_MESSAGE_MAP(COptionsPage, CRhinoOptionsDialogPage)
END_MESSAGE_MAP()


// COptionsPage message handlers

const wchar_t* COptionsPage::EnglishPageTitle() { return L"Test Page"; }
const wchar_t* COptionsPage::LocalPageTitle() { return EnglishPageTitle(); }
CRhinoCommand::result COptionsPage::RunScript( CRhinoDoc& rhino_doc) { return CRhinoCommand::success; }

int COptionsPage::OnActivate( BOOL bActive)
{
  // Apply changes when the page is deactivated/hidden and 
  // initialize page values when the page is activated/shown.
  // This allows multiple pages to share settings values

  // Apply changes when the page is no longer active
  if (!bActive)
    return OnApply();

  // Initialize the page when activated
  m_obj_color_button.SetColor(CApp::App().DefaultObjectColor());
  return true;
}

int COptionsPage::OnApply()
{
  // Apply the application color setting
  CApp::App().SetDefaultObjectColor(m_obj_color_button.Color());
  return true;
}

void COptionsPage::OnDefaults(void)
{
  // Set the color button to the default application object color
  m_obj_color_button.SetColor(CApp::DEFULLT_OBJECT_COLOR);
}
