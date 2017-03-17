#include "stdafx.h"
#include "SampleRhinoDockbarDialog.h"
#include "SampleUserInterfacePlugIn.h"

IMPLEMENT_SERIAL(CSampleRhinoDockbarDialog, CRhinoUiDockBarDialog, 1)

CSampleRhinoDockbarDialog::CSampleRhinoDockbarDialog(CWnd* pParent /*=NULL*/)
  : CRhinoUiDockBarDialog(CSampleRhinoDockbarDialog::IDD, pParent)
{
  m_scroller = new CSampleDialogScrollHelper();
  if (m_scroller)
    m_scroller->Attach(this);
}

CSampleRhinoDockbarDialog::~CSampleRhinoDockbarDialog()
{
  if (m_scroller)
  {
    delete m_scroller;
    m_scroller = 0;
  }
}

void CSampleRhinoDockbarDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoUiDockBarDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleRhinoDockbarDialog, CRhinoUiDockBarDialog)
  ON_WM_HSCROLL()
  ON_WM_VSCROLL()
  ON_WM_MOUSEWHEEL()
  ON_WM_SIZE()
  ON_BN_CLICKED(IDC_BUTTON1, &CSampleRhinoDockbarDialog::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CSampleRhinoDockbarDialog::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CSampleRhinoDockbarDialog::OnInitDialog()
{
  if (m_scroller)
  {
    CRect rect;
    GetClientRect(rect);
    m_scroller->SetDisplaySize(rect);
  }

  CRhinoUiDockBarDialog::OnInitDialog();

  // TODO...

  return TRUE;
}

void CSampleRhinoDockbarDialog::OnBnClickedButton1()
{
  MessageBox(L"Hello Rhino!", SampleUserInterfacePlugIn().PlugInName(), MB_OK);
}


void CSampleRhinoDockbarDialog::OnBnClickedButton2()
{
  MessageBox(L"Hello Rhino!", SampleUserInterfacePlugIn().PlugInName(), MB_OK);
}

void CSampleRhinoDockbarDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if (m_scroller)
    m_scroller->OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSampleRhinoDockbarDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if (m_scroller)
    m_scroller->OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSampleRhinoDockbarDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  BOOL rc = FALSE;
  if (m_scroller)
    rc = m_scroller->OnMouseWheel(nFlags, zDelta, pt);
  return rc;
}

void CSampleRhinoDockbarDialog::OnSize(UINT nType, int cx, int cy)
{
  CRhinoUiDockBarDialog::OnSize(nType, cx, cy);
  if (m_scroller)
    m_scroller->OnSize(nType, cx, cy);
}