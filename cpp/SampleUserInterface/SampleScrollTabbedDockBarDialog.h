#pragma once

#include "SampleDialogScrollHelper.h"
#include "resource.h"

class CSampleScrollTabbedDockBarDialog : public CRhinoTabbedDockBarDialog
{
  DECLARE_DYNCREATE(CSampleScrollTabbedDockBarDialog)

public:
  CSampleScrollTabbedDockBarDialog();
  virtual ~CSampleScrollTabbedDockBarDialog();

  // Required CRhinoTabbedDockBarDialog overrides
  const wchar_t* Caption() const;
  ON_UUID TabId() const;
  static ON_UUID ID();
  ON_UUID PlugInId() const;
  HICON Icon(const CSize& sizeInPixels) const;

  // Optional CRhinoTabbedDockBarDialog overrides
  void OnShowTab(bool bShowTab, const ON_UUID& tabId);

  // Dialog Data
  enum { IDD = IDD_SCROLL_DOCKBAR_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  DECLARE_MESSAGE_MAP()

private:
  CSampleDialogScrollHelper* m_scroller;
};
