#pragma once

#include "Resource.h"

class CSampleModelessDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleModelessDialog)

public:
  CSampleModelessDialog(CWnd* pParent = nullptr);

  // Dialog Data
  enum { IDD = IDD_MODELESS_DIALOG };
  CEdit m_edit1;
  CEdit m_edit2;
  CEdit m_edit3;

  ON_3dPoint m_pt1;
  ON_3dPoint m_pt2;
  ON_3dPoint m_pt3;

  void SetPointValue(int item, const ON_3dPoint& pt);
  void KillDialog();

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnBnClickedButton3();
  virtual void PostNcDestroy() override;
  virtual void OnOK() override;
  virtual void OnCancel() override;
  DECLARE_MESSAGE_MAP()
};
