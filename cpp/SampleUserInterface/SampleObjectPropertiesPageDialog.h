#pragma once

#include "Resource.h"

class CSampleObjectPropertiesPageDialog : public TRhinoPropertiesPanelPage<CRhinoDialog>
{
public:
  CSampleObjectPropertiesPageDialog();
  ~CSampleObjectPropertiesPageDialog() = default;

  RHINO_PAGE_DECLARE
  RHINO_PROPERTIES_PANEL_PAGE_DECLARE
  void OnModifyPage(IRhinoPropertiesPanelPageEventArgs& args) override;

  // Dialog data
  enum { IDD = IDD_OBJPROPS_DIALOG };
  CRhinoUiSeparator m_separator;
  CEdit	m_edit;
  CButton	m_button;
  // Extra data
  ON_wString m_hyperlink;
  bool m_bVaries;
  bool m_bDirty;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnEditChange();
  afx_msg void OnEditKillFocus();
  afx_msg void OnButton();
  DECLARE_MESSAGE_MAP()
};
