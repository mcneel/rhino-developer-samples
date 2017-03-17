#include "stdafx.h"
#include "SampleOptionsPage.h"

IMPLEMENT_DYNAMIC(CSampleOptionsPage, CRhinoOptionsDialogPage)

CSampleOptionsPage::CSampleOptionsPage(CWnd* pParent /*=NULL*/)
  : CRhinoOptionsDialogPage(CSampleOptionsPage::IDD, pParent)
{
}

CSampleOptionsPage::~CSampleOptionsPage()
{
}

void CSampleOptionsPage::DoDataExchange(CDataExchange* pDX)
{
  CRhinoOptionsDialogPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON1, m_button);
  DDX_Control(pDX, IDC_SEP1, m_sep1);
}

BEGIN_MESSAGE_MAP(CSampleOptionsPage, CRhinoOptionsDialogPage)
  ON_BN_CLICKED(IDC_BUTTON1, &CSampleOptionsPage::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CSampleOptionsPage::OnInitDialog()
{
  CRhinoOptionsDialogPage::OnInitDialog();

  return TRUE;
}

const wchar_t* CSampleOptionsPage::EnglishPageTitle()
{
  return L"Sample Options";
}

const wchar_t* CSampleOptionsPage::LocalPageTitle()
{
  return L"Sample Options";
}

void CSampleOptionsPage::RhinoDeleteThisPage()
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

int CSampleOptionsPage::OnApply()
{
  return TRUE;
}

CRhinoCommand::result CSampleOptionsPage::RunScript(const unsigned int rhino_doc_sn)
{
  UNREFERENCED_PARAMETER(rhino_doc_sn);
  return CRhinoCommand::success;
}

void CSampleOptionsPage::OnBnClickedButton1()
{
  RhinoMessageBox(GetSafeHwnd(), L"Pick me button pressed", LocalPageTitle(), MB_OK);
}
