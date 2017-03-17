#include "stdafx.h"
#include "SampleModalDialog.h"
#include "SampleUserInterfacePlugIn.h"
#include "SampleModelessDialog.h"

IMPLEMENT_DYNAMIC(CSampleModelessDialog, CRhinoDialog)

CSampleModelessDialog::CSampleModelessDialog(CWnd* pParent /*=NULL*/)
  : CRhinoDialog(CSampleModelessDialog::IDD, pParent)
{
  m_pt1 = ON_3dPoint(0, 0, 0);
  m_pt2 = ON_3dPoint(0, 0, 0);
  m_pt3 = ON_3dPoint(0, 0, 0);
}

CSampleModelessDialog::~CSampleModelessDialog()
{
}

void CSampleModelessDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT1, m_edit1);
  DDX_Control(pDX, IDC_EDIT2, m_edit2);
  DDX_Control(pDX, IDC_EDIT3, m_edit3);
}

BEGIN_MESSAGE_MAP(CSampleModelessDialog, CRhinoDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &CSampleModelessDialog::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CSampleModelessDialog::OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON3, &CSampleModelessDialog::OnBnClickedButton3)
END_MESSAGE_MAP()


BOOL CSampleModelessDialog::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  SetPointValue(1, m_pt1);
  SetPointValue(2, m_pt2);
  SetPointValue(3, m_pt3);

  return TRUE;
}

void CSampleModelessDialog::SetPointValue(int item, const ON_3dPoint& pt)
{
  ON_wString str;
  if (item == 1)
  {
    m_pt1 = pt;
    RhinoFormatPoint(m_pt1, str, L"%0.2f");
    m_edit1.SetWindowText(str);
  }
  else if (item == 2)
  {
    m_pt2 = pt;
    RhinoFormatPoint(m_pt2, str, L"%0.2f");
    m_edit2.SetWindowText(str);
  }
  else
  {
    m_pt3 = pt;
    RhinoFormatPoint(m_pt3, str, L"%0.2f");
    m_edit3.SetWindowText(str);
  }
}

void CSampleModelessDialog::KillDialog()
{
  OnCancel();
}

void CSampleModelessDialog::OnOK()
{
  OnCancel();
}

void CSampleModelessDialog::OnCancel()
{
  SampleUserInterfacePlugIn().ZeroDlg();
  CRhinoDialog::OnCancel();
}

void CSampleModelessDialog::PostNcDestroy()
{
  CRhinoDialog::PostNcDestroy();
  delete this;
}

void CSampleModelessDialog::OnBnClickedButton1()
{
  RhinoApp().RunScript(L"! _GetPoint1", 0);
}

void CSampleModelessDialog::OnBnClickedButton2()
{
  RhinoApp().RunScript(L"! _GetPoint2", 0);
}

void CSampleModelessDialog::OnBnClickedButton3()
{
  RhinoApp().RunScript(L"! _GetPoint3", 0);
}

