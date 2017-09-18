// DocumentPropertiesPage.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "DocumentPropertiesPage.h"

// CDocumentPropertiesPage dialog

IMPLEMENT_DYNAMIC(CDocumentPropertiesPage, CRhinoOptionsDialogPage)

CDocumentPropertiesPage::CDocumentPropertiesPage(CRhinoDoc& doc)
: CRhinoOptionsDialogPage(CDocumentPropertiesPage::IDD)
, m_doc(doc)
{

}

CDocumentPropertiesPage::~CDocumentPropertiesPage()
{
}

void CDocumentPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
  CRhinoOptionsDialogPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON1, m_color_button);
}


BEGIN_MESSAGE_MAP(CDocumentPropertiesPage, CRhinoOptionsDialogPage)
END_MESSAGE_MAP()


// CDocumentPropertiesPage message handlers

const wchar_t* CDocumentPropertiesPage::EnglishPageTitle() { return L"Test Page"; }
const wchar_t* CDocumentPropertiesPage::LocalPageTitle() { return EnglishPageTitle(); }
CRhinoCommand::result CDocumentPropertiesPage::RunScript( CRhinoDoc& rhino_doc) { return CRhinoCommand::success; }

int CDocumentPropertiesPage::OnActivate( BOOL bActive)
{
  // Applychanges when the page is deactivated/hidden and 
  // initialize page values when the page is activated/shown.
  // This allows multiple pages to share settings values

  // Apply changes when the page is no longer active
  if (!bActive)
    return OnApply();

  // Initialize the page when activated
  m_color_button.SetColor(CApp::App().ObjectColor(&m_doc));
  return true;
}

int CDocumentPropertiesPage::OnApply()
{
  // Set the document specific object color
  CApp::App().SetObjectColor(&m_doc, m_color_button.Color());
  return true;
}

void CDocumentPropertiesPage::OnDefaults(void)
{
  // Set the color button to the default application object color
  m_color_button.SetColor(CApp::DEFULLT_OBJECT_COLOR);
}
