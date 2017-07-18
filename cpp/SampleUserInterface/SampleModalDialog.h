#pragma once

#include "resource.h"

class CSampleModalDialog : public CDialog
{
  DECLARE_DYNAMIC(CSampleModalDialog)

public:
  CSampleModalDialog(CWnd* pParent = 0);   // standard constructor

  // Dialog Data
  enum { IDD = IDD_MODAL_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
  DECLARE_MESSAGE_MAP()
};
