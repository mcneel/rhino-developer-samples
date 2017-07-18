#pragma once

#include "Resource.h"

class CSampleBlockViewDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleBlockViewDialog)

public:
  CSampleBlockViewDialog(CWnd* pParent, CRhinoDoc& doc);

  // Dialog Data
  enum { IDD = IDD_BLOCKVIEW_DIALOG };
  CListBox m_listbox;
  CRhinoUiBlockPreview m_preview;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnSelChangeListBox();
  DECLARE_MESSAGE_MAP()

protected:
  CRhinoDoc& m_doc;
};
