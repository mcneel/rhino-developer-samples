#include "stdafx.h"
#include "SampleUserInterfacePlugIn.h"
#include "SampleScrollTabbedDockBarDialog.h"

IMPLEMENT_DYNCREATE(CSampleScrollTabbedDockBarDialog, CRhinoTabbedDockBarDialog)

CSampleScrollTabbedDockBarDialog::CSampleScrollTabbedDockBarDialog()
  : CRhinoTabbedDockBarDialog()
{
  m_scroller = new CSampleDialogScrollHelper();
  if (m_scroller)
    m_scroller->Attach(this);
}

CSampleScrollTabbedDockBarDialog::~CSampleScrollTabbedDockBarDialog()
{
  if (m_scroller)
  {
    delete m_scroller;
    m_scroller = 0;
  }
}

void CSampleScrollTabbedDockBarDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoTabbedDockBarDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleScrollTabbedDockBarDialog, CRhinoTabbedDockBarDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &CSampleScrollTabbedDockBarDialog::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CSampleScrollTabbedDockBarDialog::OnBnClickedButton2)
  ON_WM_HSCROLL()
  ON_WM_VSCROLL()
  ON_WM_MOUSEWHEEL()
  ON_WM_SIZE()
END_MESSAGE_MAP()

const wchar_t* CSampleScrollTabbedDockBarDialog::Caption() const
{
  return L"ScrollTab";
}

ON_UUID CSampleScrollTabbedDockBarDialog::TabId() const
{
  return ID();
}

ON_UUID CSampleScrollTabbedDockBarDialog::ID()
{
  // {428BF9CD-C7F4-4A69-A6B3-B271B2BA7528}
  static const GUID SampleScrollTabbedDockBarDialog_UUID =
  { 0x428BF9CD, 0xC7F4, 0x4A69, { 0xA6, 0xB3, 0xB2, 0x71, 0xB2, 0xBA, 0x75, 0x28 } };
  return SampleScrollTabbedDockBarDialog_UUID;
}

ON_UUID CSampleScrollTabbedDockBarDialog::PlugInId() const
{
  return SampleUserInterfacePlugIn().PlugInID();
}

HICON CSampleScrollTabbedDockBarDialog::Icon(const CSize& sizeInPixels) const
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return CRhinoDpi::LoadIcon(AfxGetInstanceHandle(), IDI_SCROLL_DOCKBAR_ICON, sizeInPixels.cx, sizeInPixels.cy);
}

void CSampleScrollTabbedDockBarDialog::OnShowTab(bool bShowTab, const ON_UUID& tabId)
{
  UNREFERENCED_PARAMETER(bShowTab);
  UNREFERENCED_PARAMETER(tabId);

  // TODO: Add "On Show" or "On Hide" tab code here
}

BOOL CSampleScrollTabbedDockBarDialog::OnInitDialog()
{
  if (m_scroller)
  {
    CRect rect;
    GetClientRect(rect);
    m_scroller->SetDisplaySize(rect);
  }

  m_Resize.Add(IDC_BUTTON1, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_BUTTON2, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);

  CRhinoTabbedDockBarDialog::OnInitDialog();

  return TRUE;
}

void CSampleScrollTabbedDockBarDialog::OnBnClickedButton1()
{
}


void CSampleScrollTabbedDockBarDialog::OnBnClickedButton2()
{
}

void CSampleScrollTabbedDockBarDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if (m_scroller)
    m_scroller->OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSampleScrollTabbedDockBarDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if (m_scroller)
    m_scroller->OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSampleScrollTabbedDockBarDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  BOOL rc = FALSE;
  if (m_scroller)
    rc = m_scroller->OnMouseWheel(nFlags, zDelta, pt);
  return rc;
}

void CSampleScrollTabbedDockBarDialog::OnSize(UINT nType, int cx, int cy)
{
  CRhinoTabbedDockBarDialog::OnSize(nType, cx, cy);
  if (m_scroller)
    m_scroller->OnSize(nType, cx, cy);
}