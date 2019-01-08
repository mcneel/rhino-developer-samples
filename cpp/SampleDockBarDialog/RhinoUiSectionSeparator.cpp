/////////////////////////////////////////////////////////////////////////////
// RhinoUiSectionSeparator.cpp

#include "StdAfx.h"
#include "rhinoSdkUiSectionSeparator.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CRhinoUiSectionSeparator, CRhinoUiStatic)

CRhinoUiSectionSeparator::CRhinoUiSectionSeparator()
{
  m_bChanging = false;
  m_bCollapsed = false;
  m_bResizeParent = true;
  m_nSectionHeight = 0;
}

BEGIN_MESSAGE_MAP(CRhinoUiSectionSeparator, CRhinoUiStatic)
  ON_WM_ENABLE()
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_SIZE()
END_MESSAGE_MAP()

void CRhinoUiSectionSeparator::PreSubclassWindow()
{
  CRhinoUiStatic::PreSubclassWindow();

  // Allow control to receive mouse inputs
  ModifyStyle(0, SS_NOTIFY, 0);

  // 7-Dec-2015 Dale Fugier, DPI-aware
  unsigned int icon_ids[4] =
  {
    IDI_TREE_ARROW_COLLAPSED,
    IDI_TREE_ARROW_COLLAPSED_GRAY,
    IDI_TREE_ARROW_EXPANDED,
    IDI_TREE_ARROW_EXPANDED_GRAY
  };

  const int size = CRhinoDpi::IconSize(CRhinoDpi::IconType::SmallIcon);
  CRhinoDpi::CreateImageList(m_imagelist, AfxGetInstanceHandle(), 4, icon_ids, size);
  m_rcImage = CRect(0, 0, size, size);
}

void CRhinoUiSectionSeparator::OnEnable(BOOL bEnable)
{
  CRhinoUiStatic::OnEnable(bEnable);
  Invalidate();
}

void CRhinoUiSectionSeparator::OnSize(UINT nType, int cx, int cy)
{
  CRhinoUiStatic::OnSize(nType, cx, cy);
  Invalidate();
}

BOOL CRhinoUiSectionSeparator::OnEraseBkgnd(CDC*)
{
  return TRUE;
}

void CRhinoUiSectionSeparator::OnPaint()
{
  CPaintDC paint_dc(this);

  CRect rcWnd;
  GetClientRect(rcWnd);
  CRect rcText = rcWnd;

  CRhinoUiMemDC dc(&paint_dc, &rcWnd);

  HBRUSH hBrush = 0;
  CWnd* pWndParent = GetParent();
  if (pWndParent)
  {
    CDC* pParentDC = pWndParent->GetDC();
    if (pParentDC)
    {
      hBrush = (HBRUSH)pWndParent->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)pParentDC->m_hDC, (LPARAM)pWndParent->m_hWnd);
      pWndParent->ReleaseDC(pParentDC);
    }
  }

  if (0 == hBrush)
    hBrush = (HBRUSH)SendMessage(WM_CTLCOLORSTATIC, (WPARAM)paint_dc.m_hDC, (LPARAM)m_hWnd);
  if (hBrush)
    ::FillRect(dc, rcWnd, hBrush);
  else
    dc.FillSolidRect(rcWnd, ::GetSysColor(COLOR_BTNFACE));

  if (rcWnd.Height() > rcWnd.Width())
  {
    dc.Draw3dRect(rcWnd.Width() / 2, 0, 2, rcWnd.Height(), ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
  }
  else
  {
    DWORD dwStyle = GetStyle();
    UINT uFormat = DT_TOP | DT_END_ELLIPSIS;
    if (dwStyle & SS_NOPREFIX)
      uFormat |= DT_NOPREFIX;
    uFormat |= DT_LEFT;

    CString strText;
    GetWindowText(strText);

    if (TextIsFilePath())
      uFormat |= DT_END_ELLIPSIS | DT_PATH_ELLIPSIS;

    CFont* pFontOld = dc.SelectObject(GetFont());
    COLORREF cfTextColor = dc.GetTextColor();

    bool bDisabled = ((GetStyle() & WS_DISABLED) != 0);
    if (bDisabled)
      dc.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
    else
    {
      dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));

      CRhinoUiThemeDraw td(m_hWnd, L"BUTTON");
      if (td.IsInitialized())
      {
        COLORREF crThemeText = dc.GetTextColor();
        //int iState = PBS_NORMAL;
        if (td.GetThemeColor(BP_GROUPBOX, bDisabled ? PBS_DISABLED : PBS_NORMAL, TMT_TEXTCOLOR, &crThemeText))
          dc.SetTextColor(crThemeText);
      }
    }

    int nImage = 0;
    if (bDisabled)
      nImage = IsCollapsed() ? collapsed_disabled : expanded_disabled;
    else
      nImage = IsCollapsed() ? collapse_enabled : expanded_enabled;

    // Draw icon
    CPoint pt(0, (rcText.Height() - m_rcImage.Height()) / 2);
    m_imagelist.Draw(&dc, nImage, pt, ILD_TRANSPARENT);

    // Offset and draw text
    rcText.OffsetRect(m_rcImage.Width() + 4, 0);
    int prev_bk_mode = dc.SetBkMode(TRANSPARENT);
    dc.DrawText(strText, rcText, uFormat);

    CRect rcTextSize = rcText;
    dc.DrawText(strText, rcTextSize, uFormat | DT_CALCRECT);
    rcTextSize.InflateRect(4, 4); // add padding for empty area

    // Reset
    dc.SelectObject(pFontOld);
    dc.SetTextColor(cfTextColor);
    dc.SetBkMode(prev_bk_mode);

    int top = rcWnd.Height() / 2;
    int bottom = top + 2;

    int left = rcTextSize.right;
    int right = rcWnd.right;
    CRect rcRightRect(left, top, right, bottom);
    dc.Draw3dRect(&rcRightRect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
  }
}

bool CRhinoUiSectionSeparator::IsCollapsed()
{
  return m_bCollapsed;
}

void CRhinoUiSectionSeparator::SetResizeParent(bool bResize)
{
  m_bResizeParent = bResize;
}

void CRhinoUiSectionSeparator::SetNextSectionCtrl(const CWnd& wnd)
{
  m_nNextSectionId = wnd.GetDlgCtrlID();
  UpdateSectionHeight();
}

void CRhinoUiSectionSeparator::SetNextSectionCtrl(int nId)
{
  m_nNextSectionId = nId;
  UpdateSectionHeight();
}

void CRhinoUiSectionSeparator::UpdateSectionHeight()
{
  CWnd* pWndParent = GetParent();
  if (nullptr == pWndParent)
    return;

  CRect rc1;
  GetWindowRect(&rc1);
  pWndParent->ScreenToClient(&rc1);

  CRect rc2;
  if (pWndParent->GetDlgItem(m_nNextSectionId))
  {
    pWndParent->GetDlgItem(m_nNextSectionId)->GetWindowRect(&rc2);
    pWndParent->ScreenToClient(&rc2);
  }
  else
  {
    pWndParent->GetClientRect(&rc2);
    rc2.top = rc2.bottom;
  }

  m_nSectionHeight = rc2.top - rc1.bottom - 5;
  ASSERT(m_nSectionHeight > 0);
}

void CRhinoUiSectionSeparator::GetSectionTops(int& nCurrTop, int& nNextTop)
{
  CWnd* pWndParent = GetParent();
  if (nullptr == pWndParent)
    return;

  // Current section
  CRect rc;
  GetWindowRect(&rc);
  pWndParent->ScreenToClient(&rc);
  nCurrTop = rc.top;

  // next section
  if (pWndParent->GetDlgItem(m_nNextSectionId))
  {
    pWndParent->GetDlgItem(m_nNextSectionId)->GetWindowRect(&rc);
    pWndParent->ScreenToClient(&rc);
  }
  else
  {
    pWndParent->GetClientRect(&rc);
    rc.top = rc.bottom;
  }
  nNextTop = rc.top;
}

void CRhinoUiSectionSeparator::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (!m_bChanging)
  {
    CRect rc;
    GetClientRect(&rc);
    if (rc.PtInRect(point))
    {
      if (m_bCollapsed)
        Expand();
      else
        Collapse();
    }
  }

  CRhinoUiStatic::OnLButtonDown(nFlags, point);
}

void CRhinoUiSectionSeparator::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  if (!m_bChanging)
  {
    CRect rc;
    GetClientRect(&rc);
    if (rc.PtInRect(point))
    {
      if (m_bCollapsed)
        Expand();
      else
        Collapse();
    }
  }

  CRhinoUiStatic::OnLButtonDblClk(nFlags, point);
}

void CRhinoUiSectionSeparator::Collapse()
{
  if (!m_bCollapsed && m_nSectionHeight)
  {
    CWnd* pWndParent = GetParent();
    if (nullptr == pWndParent)
      return;

    m_bChanging = true;

    // Get the tops
    int nSectionTop = 0;
    int nNextSectionTop = 0;
    GetSectionTops(nSectionTop, nNextSectionTop);

    CActions actions;

    // Iterate the children
    CWnd* pChild = pWndParent->GetWindow(GW_CHILD);
    while (pChild)
    {
      // Ignore ourself
      if (pChild == this)
      {
        pChild = pChild->GetWindow(GW_HWNDNEXT);
        continue;
      }

      CRect rc;
      pChild->GetWindowRect(&rc);
      pWndParent->ScreenToClient(&rc);

      // Ignore controls above
      if (rc.top < nSectionTop)
      {
        pChild = pChild->GetWindow(GW_HWNDNEXT);
        continue;
      }

      // Hide controls above options end
      if (rc.bottom < nNextSectionTop)
      {
        m_related[pChild->GetSafeHwnd()] = rc.top - nSectionTop;
        pChild->ShowWindow(SW_HIDE);
        pChild->MoveWindow(rc.left, nSectionTop, rc.Width(), rc.Height());
      }
      else
      {
        // move
        actions[pChild->GetSafeHwnd()] = CRect(0, -m_nSectionHeight, 0, 0);
      }

      pChild = pChild->GetWindow(GW_HWNDNEXT);
    }

    // Update size of dialog
    if (m_bResizeParent)
      actions[pWndParent->GetSafeHwnd()] = CRect(0, 0, 0, -m_nSectionHeight);

    // Move the controls
    MoveControls(actions);

    // update icon
    m_bCollapsed = true;
    m_bChanging = false;
    pWndParent->RedrawWindow();
  }
}

void CRhinoUiSectionSeparator::Expand()
{
  if (m_bCollapsed && m_nSectionHeight)
  {
    CWnd* pWndParent = GetParent();
    if (nullptr == pWndParent)
      return;

    m_bChanging = true;

    // Get the tops
    int nSectionTop = 0;
    int nNextSectionTop = 0;
    GetSectionTops(nSectionTop, nNextSectionTop);

    CActions actions;

    // Update size of dialog
    if (m_bResizeParent)
      actions[pWndParent->GetSafeHwnd()] = CRect(0, 0, 0, m_nSectionHeight);

    // Iterate on children
    CWnd* pChild = pWndParent->GetWindow(GW_CHILD);
    while (pChild)
    {
      // Ignore ourself
      if (pChild == this)
      {
        pChild = pChild->GetWindow(GW_HWNDNEXT);
        continue;
      }

      // Skip related controls
      int lookup = 0;
      if (m_related.Lookup(pChild->GetSafeHwnd(), lookup))
      {
        pChild = pChild->GetWindow(GW_HWNDNEXT);
        continue;
      }

      // Get rect and move
      CRect rc;
      pChild->GetWindowRect(&rc);
      pWndParent->ScreenToClient(&rc);

      // Do not touch controls above us
      if (rc.top < nSectionTop)
      {
        pChild = pChild->GetWindow(GW_HWNDNEXT);
        continue;
      }

      actions[pChild->GetSafeHwnd()] = CRect(0, m_nSectionHeight, 0, 0);
      pChild = pChild->GetWindow(GW_HWNDNEXT);
    }

    // Move the controls
    MoveControls(actions);

    // Show the related controls
    CRelated::CPair* pPair = m_related.PGetFirstAssoc();
    while (pPair)
    {
      // Get rect and move
      CRect rc;
      pChild = CWnd::FromHandle(pPair->key);
      pChild->GetWindowRect(&rc);
      pWndParent->ScreenToClient(&rc);
      pChild->MoveWindow(rc.left, nSectionTop + pPair->value, rc.Width(), rc.Height());
      pChild->ShowWindow(SW_SHOW);
      pPair = m_related.PGetNextAssoc(pPair);
    }

    // update icon
    m_bCollapsed = false;
    m_bChanging = false;
    pWndParent->RedrawWindow();
  }
}

void CRhinoUiSectionSeparator::MoveControls(CActions& actions)
{
  CWnd* pWndParent = GetParent();
  if (nullptr == pWndParent)
    return;

  HWND hwndParent = pWndParent->GetSafeHwnd();

  bool bResizeParent = false;
  int nParentOrigWidth = 0;
  int nParentOrigHeight = 0;
  int nParentWidthInc = 0;
  int nParentHeightInc = 0;

  // Save the initial rect of each control
  CActions initial;
  CActions::CPair* pPair = actions.PGetFirstAssoc();
  while (pPair)
  {
    CRect rc;
    CWnd* pWnd = CWnd::FromHandle(pPair->key);
    pWnd->GetWindowRect(&rc);
    if (pPair->key != hwndParent)
    {
      pWndParent->ScreenToClient(&rc);
      initial[pPair->key] = rc;
    }
    else
    {
      bResizeParent = true;
      nParentOrigWidth = rc.Width();
      nParentOrigHeight = rc.Height();
      nParentWidthInc = pPair->value.right;
      nParentHeightInc = pPair->value.bottom;
    }
    pPair = actions.PGetNextAssoc(pPair);
  }

  // Position everyone precisely
  HDWP hdwp = BeginDeferWindowPos(int(actions.GetCount()));

  pPair = actions.PGetFirstAssoc();
  while (pPair)
  {
    CRect rc;
    initial.Lookup(pPair->key, rc);
    rc.left += pPair->value.left;
    rc.top += pPair->value.top;
    rc.right += pPair->value.left + pPair->value.right;
    rc.bottom += pPair->value.top + pPair->value.bottom;
    if (pPair->key != hwndParent)
      hdwp = DeferWindowPos(hdwp, pPair->key, 0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOREDRAW);
    pPair = actions.PGetNextAssoc(pPair);
  }

  EndDeferWindowPos(hdwp);

  if (bResizeParent)
  {
    CRect rcParent;
    pWndParent->GetWindowRect(&rcParent);
    rcParent.right = rcParent.left + nParentOrigWidth + nParentWidthInc;
    rcParent.bottom = rcParent.top + nParentOrigHeight + nParentHeightInc;
    pWndParent->MoveWindow(rcParent, TRUE);
  }
}
