#pragma once

#include "Resource.h"

class CSamplePickObjectsDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSamplePickObjectsDialog)

public:
  CSamplePickObjectsDialog(CWnd* pParent, unsigned int document_sn);
  virtual ~CSamplePickObjectsDialog();

  // Dialog Data
  enum { IDD = IDD_PICKOBJECTS_DIALOG };
  CStatic m_message;

protected:
  virtual BOOL OnInitDialog();
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg void OnBnClickedPick();
  DECLARE_MESSAGE_MAP()
};
