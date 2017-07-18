#pragma once

#include "resource.h"

class CSampleTabbedDockBarDialog : public CRhinoTabbedDockBarDialog
{
  DECLARE_DYNCREATE(CSampleTabbedDockBarDialog)

public:
  CSampleTabbedDockBarDialog();
  virtual ~CSampleTabbedDockBarDialog();

  // Required CRhinoTabbedDockBarDialog overrides
  const wchar_t* Caption() const override;
  ON_UUID TabId() const override;
  static ON_UUID ID();
  ON_UUID PlugInId() const override;

  // Optional CRhinoTabbedDockBarDialog overrides
  void OnShowTab(const class CRhinoUiPanel& db, bool bShowTab, const ON_UUID& dockBarId) override;

  // Dialog Data
  enum { IDD = IDD_DOCKBAR_DIALOG };
  CButton	m_clickme_button;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnClickedClickMeButton();
  DECLARE_MESSAGE_MAP()
};
