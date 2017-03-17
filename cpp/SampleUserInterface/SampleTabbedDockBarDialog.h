#pragma once

#include "resource.h"

class CSampleTabbedDockBarDialog : public CRhinoTabbedDockBarDialog
{
  DECLARE_DYNCREATE(CSampleTabbedDockBarDialog)

public:
  CSampleTabbedDockBarDialog();
  virtual ~CSampleTabbedDockBarDialog();

  // Required CRhinoTabbedDockBarDialog overrides
  const wchar_t* Caption() const;
  ON_UUID TabId() const;
  static ON_UUID ID();
  ON_UUID PlugInId() const;
  HICON Icon(const CSize& sizeInPixels) const;

  // Optional CRhinoTabbedDockBarDialog overrides
  void OnShowTab(bool bShowTab, const ON_UUID& tabId);

  // Dialog Data
  enum { IDD = IDD_DOCKBAR_DIALOG };
  CButton	m_clickme_button;

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();
  afx_msg void OnClickedClickMeButton();
  DECLARE_MESSAGE_MAP()
};
