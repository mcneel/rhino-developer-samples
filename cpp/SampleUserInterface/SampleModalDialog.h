#pragma once

#include "resource.h"

class CSampleModalDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleModalDialog)

public:
  CSampleModalDialog(CWnd* pParent = nullptr);

  // Dialog Data
  enum { IDD = IDD_MODAL_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  DECLARE_MESSAGE_MAP()
};
