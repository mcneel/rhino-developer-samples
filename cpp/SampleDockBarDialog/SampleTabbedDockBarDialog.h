#pragma once

#include "rhinoSdkUiSectionSeparator.h"
#include "rhinoSdkUiDialogScrollHelper.h"
#include "resource.h"

// CSampleTabbedDockBarDialog dialog

class CSampleTabbedDockBarDialog : public CRhinoTabbedDockBarDialog
{
  DECLARE_DYNCREATE(CSampleTabbedDockBarDialog)

public:
	CSampleTabbedDockBarDialog();
  virtual ~CSampleTabbedDockBarDialog();

  static ON_UUID Id();

  // Required overrides
  const wchar_t* Caption() const override;
  ON_UUID TabId() const override;
  ON_UUID PlugInId() const override;

// Dialog Data
	enum { IDD = IDD_DIALOG };
  CRhinoUiSectionSeparator m_static1;
  CButton m_check1;
  CButton m_check2;
  CButton m_check3;
  CButton m_check4;
  CRhinoUiSectionSeparator m_static2;
  CButton m_check5;
  CButton m_check6;
  CButton m_check7;
  CButton m_check8;
  CRhinoUiSectionSeparator m_static3;
  CButton m_check9;
  CButton m_check10;
  CButton m_check11;
  CButton m_check12;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  DECLARE_MESSAGE_MAP()

private:
  CRhinoUiDialogScrollHelper* m_scroller;
};
