#pragma once

#include "Resource.h"

class CSampleDialog1 : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleDialog1)

public:
  CSampleDialog1(CWnd* pParentWnd, CRhinoModelessDialogCommand* pCommand);
  virtual ~CSampleDialog1();

  // Dialog Data
  enum { IDD = IDD_DIALOG1 };

  void DisposeDialog();

public:
  afx_msg void OnGetButton();
  afx_msg void OnCustomButton();
  afx_msg void OnPointsButton();
  afx_msg void OnCancel1Button();
  afx_msg void OnCancel2Button();
  afx_msg void OnCancel3Button();
  afx_msg void OnCloseButton();

protected:
  virtual BOOL OnInitDialog() override;
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual void PostNcDestroy() override;
  DECLARE_MESSAGE_MAP()

private:
  CRhinoModelessDialogCommand* m_pCommand = nullptr;
};
