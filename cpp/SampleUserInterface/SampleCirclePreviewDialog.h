#pragma once

#include "Resource.h"

class CSampleCirclePreviewConduit : public CRhinoDisplayConduit
{
public:
  CSampleCirclePreviewConduit();

  bool ExecConduit(
    CRhinoDisplayPipeline& dp, // pipeline executing this conduit
    UINT nChannel,             // current channel within the pipeline
    bool& bTerminate           // channel termination flag
  ) override;

  ON_Circle m_circle;
};


class CSampleCirclePreviewDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleCirclePreviewDialog)

public:
  CSampleCirclePreviewDialog(CWnd* pParent, unsigned int document_sn);

  // Dialog Data
  enum { IDD = IDD_CIRCLEPREVIEW_DIALOG };
  CRhinoUiSeparator m_sep1;
  CRhinoUiSeparator m_sep2;
  CRhinoUiEdit m_edit_center_x;
  CRhinoUiEdit m_edit_center_y;
  CRhinoUiEdit m_edit_center_z;
  CRhinoUiEdit m_edit_radius;
  double m_center_x;
  double m_center_y;
  double m_center_z;
  double m_radius;

  CSampleCirclePreviewConduit m_preview;
  unsigned int m_document_sn;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnBnClickedPreview();
  afx_msg void OnBnClickedCancel();
  DECLARE_MESSAGE_MAP()

};
