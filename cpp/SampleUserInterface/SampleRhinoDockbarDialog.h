#pragma once

#include "Resource.h"
#include "SampleDialogScrollHelper.h"

class CSampleRhinoDockbarDialog : public CRhinoUiDockBarDialog
{
  DECLARE_SERIAL(CSampleRhinoDockbarDialog)

public:
  CSampleRhinoDockbarDialog(CWnd* pParent = NULL);
  virtual ~CSampleRhinoDockbarDialog();

  // Dialog Data
  enum { IDD = IDD_RHINO_DOCKBAR_DIALOG };

protected:
  virtual BOOL OnInitDialog();
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  DECLARE_MESSAGE_MAP()

private:
  CSampleDialogScrollHelper* m_scroller;
};
