#pragma once

#include "Resource.h"

class CSampleBlockViewDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleBlockViewDialog)

public:
  CSampleBlockViewDialog(CWnd* pParent, CRhinoDoc& doc);
  virtual ~CSampleBlockViewDialog();

  // Dialog Data
  enum { IDD = IDD_BLOCKVIEW_DIALOG };
  CListBox m_listbox;
  CRhinoUiBlockPreview m_preview;

public:
  virtual BOOL OnInitDialog();
  afx_msg void OnSelChangeListBox();

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  DECLARE_MESSAGE_MAP()

  void CreateTopViewport(CRhinoViewport& vp);
  void ZoomExtentsViewport(CRhinoViewport& vp);
  CRect GetPaintRect();

protected:
  CRhinoDoc& m_doc;
};
