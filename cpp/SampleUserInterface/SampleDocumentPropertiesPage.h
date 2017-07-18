#pragma once

#include "Resource.h"

class CSampleDocumentPropertiesPage : public TRhinoOptionsPage<CRhinoDialog>
{
public:
  CSampleDocumentPropertiesPage();
  ~CSampleDocumentPropertiesPage() = default;

  RHINO_PAGE_DECLARE
  RHINO_OPTIONS_PAGE_DECLARE

  // Dialog Data
  enum { IDD = IDD_DOCPROPS_PAGE };
  CRhinoUiSeparator m_sep1;
  CButton m_button;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnBnClickedButton1();
  DECLARE_MESSAGE_MAP()
};