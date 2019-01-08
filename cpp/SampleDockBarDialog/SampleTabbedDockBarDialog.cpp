///////////////////////////////////////////////////////////////////////////////
// SampleTabbedDockBarDialog.cpp : implementation file

#include "stdafx.h"
#include "SampleTabbedDockBarDialog.h"
#include "SampleDockBarDialogPlugIn.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CSampleTabbedDockBarDialog, CRhinoTabbedDockBarDialog)

CSampleTabbedDockBarDialog::CSampleTabbedDockBarDialog()
	: CRhinoTabbedDockBarDialog()
{
  m_scroller = new CRhinoUiDialogScrollHelper();
  if (m_scroller)
    m_scroller->Attach(this);
}

CSampleTabbedDockBarDialog::~CSampleTabbedDockBarDialog()
{
  if (m_scroller)
  {
    delete m_scroller;
    m_scroller = nullptr;
  }
}

void CSampleTabbedDockBarDialog::DoDataExchange(CDataExchange* pDX)
{
	CRhinoTabbedDockBarDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STATIC1, m_static1);
  DDX_Control(pDX, IDC_CHECK1, m_check1);
  DDX_Control(pDX, IDC_CHECK2, m_check2);
  DDX_Control(pDX, IDC_CHECK3, m_check3);
  DDX_Control(pDX, IDC_CHECK4, m_check4);
  DDX_Control(pDX, IDC_STATIC2, m_static2);
  DDX_Control(pDX, IDC_CHECK5, m_check5);
  DDX_Control(pDX, IDC_CHECK6, m_check6);
  DDX_Control(pDX, IDC_CHECK7, m_check7);
  DDX_Control(pDX, IDC_CHECK8, m_check8);
  DDX_Control(pDX, IDC_STATIC3, m_static3);
  DDX_Control(pDX, IDC_CHECK9, m_check9);
  DDX_Control(pDX, IDC_CHECK10, m_check10);
  DDX_Control(pDX, IDC_CHECK11, m_check11);
  DDX_Control(pDX, IDC_CHECK12, m_check12);
}

BEGIN_MESSAGE_MAP(CSampleTabbedDockBarDialog, CRhinoTabbedDockBarDialog)
  ON_WM_HSCROLL()
  ON_WM_VSCROLL()
  ON_WM_MOUSEWHEEL()
  ON_WM_SIZE()
END_MESSAGE_MAP()

const wchar_t* CSampleTabbedDockBarDialog::Caption() const
{
  return L"Sample";
}

ON_UUID CSampleTabbedDockBarDialog::TabId() const
{
  return Id();
}

ON_UUID CSampleTabbedDockBarDialog::Id()
{
  // {E1BDB83A-6FD9-4C7D-87BE-9DC9AC79B578}
  static const GUID SampleObjectManagerDialog_UUID =
  { 0xe1bdb83a, 0x6fd9, 0x4c7d, { 0x87, 0xbe, 0x9d, 0xc9, 0xac, 0x79, 0xb5, 0x78 } };
  return SampleObjectManagerDialog_UUID;
}

ON_UUID CSampleTabbedDockBarDialog::PlugInId() const
{
  return SampleDockBarDialogPlugIn().PlugInID();
}

BOOL CSampleTabbedDockBarDialog::OnInitDialog()
{
  if (m_scroller)
  {
    CRect rect;
    GetClientRect(rect);
    m_scroller->SetDisplaySize(rect);
  }

  m_Resize.Add(IDC_STATIC1, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK1, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK2, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK3, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK4, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_STATIC2, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK5, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK6, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK7, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK8, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_STATIC3, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK9, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK10, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK11, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_CHECK12, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);

  CRhinoTabbedDockBarDialog::OnInitDialog();
  
  m_static1.SetNextSectionCtrl(IDC_STATIC2);
  m_static1.SetResizeParent(false);

  m_static2.SetNextSectionCtrl(IDC_STATIC3);
  m_static2.SetResizeParent(false);

  m_static3.SetNextSectionCtrl(IDC_STATIC4);
  m_static3.SetResizeParent(false);

  return TRUE;
}

void CSampleTabbedDockBarDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if (m_scroller)
    m_scroller->OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSampleTabbedDockBarDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if (m_scroller)
    m_scroller->OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSampleTabbedDockBarDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  BOOL rc = FALSE;
  if (m_scroller)
    rc = m_scroller->OnMouseWheel(nFlags, zDelta, pt);
  return rc;
}

void CSampleTabbedDockBarDialog::OnSize(UINT nType, int cx, int cy)
{
  CRhinoTabbedDockBarDialog::OnSize(nType, cx, cy);
  if (m_scroller)
    m_scroller->OnSize(nType, cx, cy);
}