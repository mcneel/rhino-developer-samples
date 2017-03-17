#pragma once

// This helper class will add scroll bars to any dialog.
//
// References:
//   MSDN ID 262954: How to create a resizable dialog box with scroll bars in Visual C++. 
//   MSDN ID 152252: How To Get 32-bit Scroll Position During Scroll Messages.
//   "Programming Windows with MFC, 2nd Edition", by Jeff Prosise.
//
// Remarks:
//   Make sure your dialog has the WS_HSCROLL and/or WS_VSCROLL styles.
//   Your dialog should also have a "Resizing" or "None" border.

class CSampleDialogScrollHelper
{
public:
  CSampleDialogScrollHelper();
  ~CSampleDialogScrollHelper();

  // Call this in your dialog's constructor
  // For example:
  //   m_scroller = new CSampleDialogScrollHelper();
  //   if( m_scroller )
  //     m_scroller->Attach( this );
  //
  void Attach(CWnd* pWnd);
  void Detach();

  // Set the base display size. When the dialog size is smaller than the display size,
  // then scrollbars will appear. Set either the display width or height to zero if you
  // don't want to enable the scroll bar in that direction.
  // Best to call this in your OnInitDialog() member before calling the base class.
  // For example:
  //   if( m_scroller )
  //   {
  //     CRect rect;
  //     GetClientRect( rect );
  //     m_scroller->SetDisplaySize( rect );
  //   }
  //
  void SetDisplaySize(CRect rect);
  void SetDisplaySize(int width, int height);
  CSize DisplaySize() const;

  // Returns the current scroll position. This might be in OnPaint() overrides.
  CSize ScrollPos() const;
  // Returns the current page size.
  CSize PageSize() const;

  // Scroll back to top, left, or top-left corner of the window.
  void ScrollToOrigin(bool bScrollLeft, bool bScrollTop);

  // Enables or disables the scroll bars (without hiding them).
  void EnableScrollBars(bool bEnable = true);
  void EnableScrollBar(int nBar, bool bEnable = true);

  // Your dialog should implement these message handlers, and them forward
  // those messages to these handlers.
  void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  void OnSize(UINT nType, int cx, int cy);

private:
  int Get32BitScrollPos(int nBar, CScrollBar* pScrollBar);
  void UpdateScrollInfo();
  void UpdateScrollBar(int nBar, int window_size, int display_size, LONG& page_size, LONG& scroll_pos, LONG& delta_pos);

private:
  CWnd* m_pWnd;
  CSize m_page_size;
  CSize m_display_size;
  CSize m_scroll_pos;
};
