#include "stdafx.h"
#include "SampleDialogScrollHelper.h"

static void GetClientRectSB(CWnd* pWnd, CRect& client_rect)
{
  if (0 == pWnd)
    return;

  pWnd->GetClientRect(&client_rect);

  CRect window_rect;
  pWnd->GetWindowRect(&window_rect);
  pWnd->ScreenToClient(&window_rect);

  int cx_vscroll = ::GetSystemMetrics(SM_CXVSCROLL);
  int cy_vscroll = ::GetSystemMetrics(SM_CYHSCROLL);

  if (window_rect.right >= (client_rect.right + cx_vscroll))
    client_rect.right += cx_vscroll;
  if (window_rect.bottom >= (client_rect.bottom + cy_vscroll))
    client_rect.bottom += cy_vscroll;
}

// The above does not take the presence of scroll bars into account.
static void GetClientRectSB_Ex(CWnd* pWnd, CSize itemSize, CRect& client_rect)
{
  if (0 == pWnd)
    return;

  pWnd->GetClientRect(&client_rect);

  CRect window_rect;
  pWnd->GetWindowRect(&window_rect);
  pWnd->ScreenToClient(&window_rect);

  int cx_vscroll = ::GetSystemMetrics(SM_CXVSCROLL);
  int cy_vscroll = ::GetSystemMetrics(SM_CYHSCROLL);

  if (window_rect.right >= (client_rect.right + cx_vscroll))
    client_rect.right += cx_vscroll;
  if (window_rect.bottom >= (client_rect.bottom + cy_vscroll))
    client_rect.bottom += cy_vscroll;

  bool bHorz = true;
  bool bVert = true;

  if (itemSize.cy <= client_rect.bottom)
    bHorz = false;
  if (itemSize.cx <= client_rect.right)
    bVert = false;

  // Adjust client area size when scroll bars are present
  if (bHorz && !bVert)
    client_rect.right -= cx_vscroll;
  else if (bVert && !bHorz)
    client_rect.bottom -= cy_vscroll;
  else if (bVert && bHorz)
  {
    client_rect.right -= cx_vscroll;
    client_rect.bottom -= cy_vscroll;
  }
}

CSampleDialogScrollHelper::CSampleDialogScrollHelper()
  : m_pWnd(0)
{
  m_page_size = CSize(0, 0);
  m_display_size = CSize(0, 0);
  m_scroll_pos = CSize(0, 0);
}

CSampleDialogScrollHelper::~CSampleDialogScrollHelper()
{
  Detach();
}

void CSampleDialogScrollHelper::Attach(CWnd* pWnd)
{
  m_pWnd = pWnd;
}

void CSampleDialogScrollHelper::Detach()
{
  m_pWnd = 0;
}

void CSampleDialogScrollHelper::SetDisplaySize(CRect rect)
{
  SetDisplaySize(rect.Width(), rect.Height());
}

void CSampleDialogScrollHelper::SetDisplaySize(int width, int height)
{
  m_display_size = CSize(width, height);
  if (m_pWnd && ::IsWindow(m_pWnd->m_hWnd))
    UpdateScrollInfo();
}

CSize CSampleDialogScrollHelper::DisplaySize() const
{
  return m_display_size;
}

CSize CSampleDialogScrollHelper::ScrollPos() const
{
  return m_scroll_pos;
}

CSize CSampleDialogScrollHelper::PageSize() const
{
  return m_page_size;
}

void CSampleDialogScrollHelper::EnableScrollBars(bool bEnable)
{
  EnableScrollBar(SB_HORZ, bEnable);
  EnableScrollBar(SB_VERT, bEnable);
}

void CSampleDialogScrollHelper::EnableScrollBar(int nBar, bool bEnable)
{
  if (m_pWnd && ::IsWindow(m_pWnd->m_hWnd))
    m_pWnd->EnableScrollBar(nBar, bEnable ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
}

void CSampleDialogScrollHelper::ScrollToOrigin(bool bScrollLeft, bool bScrollTop)
{
  if (0 == m_pWnd)
    return;

  if (bScrollLeft)
  {
    if (m_display_size.cx > 0 && m_page_size.cx > 0 && m_scroll_pos.cx > 0)
    {
      int delta_pos = -m_scroll_pos.cx;
      m_scroll_pos.cx += delta_pos;
      m_pWnd->SetScrollPos(SB_HORZ, m_scroll_pos.cx, TRUE);
      m_pWnd->ScrollWindow(-delta_pos, 0);
    }
  }

  if (bScrollTop)
  {
    if (m_display_size.cy > 0 && m_page_size.cy > 0 && m_scroll_pos.cy > 0)
    {
      int delta_pos = -m_scroll_pos.cy;
      m_scroll_pos.cy += delta_pos;
      m_pWnd->SetScrollPos(SB_VERT, m_scroll_pos.cy, TRUE);
      m_pWnd->ScrollWindow(0, -delta_pos);
    }
  }
}

void CSampleDialogScrollHelper::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  UNREFERENCED_PARAMETER(nPos);

  if (0 == m_pWnd)
    return;

  const int line_offset = 60;

  int delta_pos = 0;
  switch (nSBCode)
  {
  case SB_LINELEFT:
    delta_pos = -line_offset;
    break;

  case SB_LINERIGHT:
    delta_pos = line_offset;
    break;

  case SB_PAGELEFT:
    delta_pos = -m_page_size.cx;
    break;

  case SB_PAGERIGHT:
    delta_pos = m_page_size.cx;
    break;

  case SB_THUMBTRACK:
    delta_pos = Get32BitScrollPos(SB_HORZ, pScrollBar) - m_scroll_pos.cx;
    break;

  case SB_THUMBPOSITION:
    delta_pos = Get32BitScrollPos(SB_HORZ, pScrollBar) - m_scroll_pos.cx;
    break;

  default:
    return;
  }

  int new_scroll_pos = m_scroll_pos.cx + delta_pos;

  if (new_scroll_pos < 0)
    delta_pos = -m_scroll_pos.cx;

  int max_scroll_pos = m_display_size.cx - m_page_size.cx;
  if (new_scroll_pos > max_scroll_pos)
    delta_pos = max_scroll_pos - m_scroll_pos.cx;

  if (delta_pos != 0)
  {
    m_scroll_pos.cx += delta_pos;
    m_pWnd->SetScrollPos(SB_HORZ, m_scroll_pos.cx, TRUE);
    m_pWnd->ScrollWindow(-delta_pos, 0);
  }
}

void CSampleDialogScrollHelper::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  UNREFERENCED_PARAMETER(nPos);

  if (0 == m_pWnd)
    return;

  const int line_offset = 60;

  int delta_pos = 0;
  switch (nSBCode)
  {
  case SB_LINEUP:
    delta_pos = -line_offset;
    break;

  case SB_LINEDOWN:
    delta_pos = line_offset;
    break;

  case SB_PAGEUP:
    delta_pos = -m_page_size.cy;
    break;

  case SB_PAGEDOWN:
    delta_pos = m_page_size.cy;
    break;

  case SB_THUMBTRACK:
    delta_pos = Get32BitScrollPos(SB_VERT, pScrollBar) - m_scroll_pos.cy;
    break;

  case SB_THUMBPOSITION:
    delta_pos = Get32BitScrollPos(SB_VERT, pScrollBar) - m_scroll_pos.cy;
    break;

  default:
    return;
  }

  int new_scroll_pos = m_scroll_pos.cy + delta_pos;

  if (new_scroll_pos < 0)
    delta_pos = -m_scroll_pos.cy;

  int max_scroll_pos = m_display_size.cy - m_page_size.cy;
  if (new_scroll_pos > max_scroll_pos)
    delta_pos = max_scroll_pos - m_scroll_pos.cy;

  if (delta_pos != 0)
  {
    m_scroll_pos.cy += delta_pos;
    m_pWnd->SetScrollPos(SB_VERT, m_scroll_pos.cy, TRUE);
    m_pWnd->ScrollWindow(0, -delta_pos);
  }
}

BOOL CSampleDialogScrollHelper::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  UNREFERENCED_PARAMETER(nFlags);
  UNREFERENCED_PARAMETER(pt);

  if (0 == m_pWnd)
    return FALSE;

  int scroll_min = 0, scroll_max = 0;
  m_pWnd->GetScrollRange(SB_VERT, &scroll_min, &scroll_max);
  if (scroll_min == scroll_max)
    return FALSE;

  int num_scroll_increments = abs(zDelta) / WHEEL_DELTA;

  int num_scroll_lines_per_increment = 0;
  ::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &num_scroll_lines_per_increment, 0);

  if (num_scroll_lines_per_increment == WHEEL_PAGESCROLL)
  {
    OnVScroll(zDelta > 0 ? SB_PAGEUP : SB_PAGEDOWN, 0, 0);
    return TRUE;
  }

  int num_scroll_lines = num_scroll_increments * num_scroll_lines_per_increment;
  num_scroll_lines = max(num_scroll_lines / 3, 1);

  int i;
  for (i = 0; i < num_scroll_lines; i++)
    OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, 0);

  return TRUE;
}

void CSampleDialogScrollHelper::OnSize(UINT nType, int cx, int cy)
{
  UNREFERENCED_PARAMETER(nType);
  UNREFERENCED_PARAMETER(cx);
  UNREFERENCED_PARAMETER(cy);

  UpdateScrollInfo();
}

int CSampleDialogScrollHelper::Get32BitScrollPos(int nBar, CScrollBar* pScrollBar)
{
  if (0 == m_pWnd)
    return 0;

  // Determine if the user scrolled a scroll bar control, on the window,
  // or scrolled the window itself.
  HWND hWndScroll = 0;
  if (0 == pScrollBar)
    hWndScroll = m_pWnd->m_hWnd;
  else
    hWndScroll = pScrollBar->m_hWnd;

  int rc = 0;
  if (hWndScroll && ::IsWindow(hWndScroll))
  {
    SCROLLINFO si;
    memset(&si, 0, sizeof(SCROLLINFO));
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_TRACKPOS;
    ::GetScrollInfo(hWndScroll, nBar, &si);
    rc = si.nTrackPos;
  }

  return rc;
}

void CSampleDialogScrollHelper::UpdateScrollInfo()
{
  if (0 == m_pWnd)
    return;

  CRect rect;
  GetClientRectSB(m_pWnd, rect);
  //GetClientRectSB_Ex( m_pWnd, m_display_size, rect );

  CSize window_size(rect.Width(), rect.Height());

  CSize delta_pos(0, 0);
  UpdateScrollBar(SB_HORZ, window_size.cx, m_display_size.cx, m_page_size.cx, m_scroll_pos.cx, delta_pos.cx);
  UpdateScrollBar(SB_VERT, window_size.cy, m_display_size.cy, m_page_size.cy, m_scroll_pos.cy, delta_pos.cy);
  if (delta_pos.cx != 0 || delta_pos.cy != 0)
    m_pWnd->ScrollWindow(delta_pos.cx, delta_pos.cy);
}

void CSampleDialogScrollHelper::UpdateScrollBar(int nBar, int window_size, int display_size, LONG& page_size, LONG& scroll_pos, LONG& delta_pos)
{
  int scroll_max = 0;
  delta_pos = 0;
  if (window_size < display_size)
  {
    scroll_max = display_size - 1;
    if (page_size > 0 && scroll_pos > 0)
      scroll_pos = (LONG)(1.0 * scroll_pos * window_size / page_size);
    page_size = window_size;
    scroll_pos = min(scroll_pos, display_size - page_size - 1);
    delta_pos = m_pWnd->GetScrollPos(nBar) - scroll_pos;
  }
  else
  {
    page_size = 0;
    scroll_pos = 0;
    delta_pos = m_pWnd->GetScrollPos(nBar);
  }

  SCROLLINFO si;
  memset(&si, 0, sizeof(SCROLLINFO));
  si.cbSize = sizeof(SCROLLINFO);
  si.fMask = SIF_ALL;
  si.nMin = 0;
  si.nMax = scroll_max;
  si.nPage = page_size;
  si.nPos = scroll_pos;
  m_pWnd->SetScrollInfo(nBar, &si, TRUE);
}
