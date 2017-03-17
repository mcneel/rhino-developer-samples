#include "stdafx.h"
#include "SampleDocumentPropertiesPage.h"

IMPLEMENT_DYNAMIC(CSampleDocumentPropertiesPage, CRhinoOptionsDialogPage)

CSampleDocumentPropertiesPage::CSampleDocumentPropertiesPage(CWnd* pParent /*=NULL*/)
  : CRhinoOptionsDialogPage(CSampleDocumentPropertiesPage::IDD, pParent)
{
}

CSampleDocumentPropertiesPage::~CSampleDocumentPropertiesPage()
{
}

void CSampleDocumentPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
  CRhinoOptionsDialogPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON1, m_button);
  DDX_Control(pDX, IDC_SEP1, m_sep1);
}

BEGIN_MESSAGE_MAP(CSampleDocumentPropertiesPage, CRhinoOptionsDialogPage)
  ON_BN_CLICKED(IDC_BUTTON1, &CSampleDocumentPropertiesPage::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CSampleDocumentPropertiesPage::OnInitDialog()
{
  CRhinoOptionsDialogPage::OnInitDialog();

  return TRUE;
}

const wchar_t* CSampleDocumentPropertiesPage::EnglishPageTitle()
{
  return L"Sample Properties";
}

const wchar_t* CSampleDocumentPropertiesPage::LocalPageTitle()
{
  return L"Sample Properties";
}

void CSampleDocumentPropertiesPage::RhinoDeleteThisPage()
{
  /*
  DO NOT DELETE THIS PAGE IN PostNcDestroy()!
  When V4 first shipped, where was a lot of confusion when and where to
  delete stacked dialog pages that were created in plug-ins. The reason
  being that if the page had never been created (you never clicked on it),
  then its PostNcDestroy() member was never called. This confusion led to
  may reported crashes. Thus in later releases, Rhino always calls the
  RhinoDeleteThisPage() member. Thus the only place you need to delete
  the page is here.
  */
  delete this;
}

int CSampleDocumentPropertiesPage::OnApply()
{
  return TRUE;
}

CRhinoCommand::result CSampleDocumentPropertiesPage::RunScript(const unsigned int rhino_doc_sn)
{
  UNREFERENCED_PARAMETER(rhino_doc_sn);
  return CRhinoCommand::success;
}

void CSampleDocumentPropertiesPage::OnBnClickedButton1()
{
  RhinoMessageBox(GetSafeHwnd(), L"Pick me button pressed", LocalPageTitle(), MB_OK);
}

