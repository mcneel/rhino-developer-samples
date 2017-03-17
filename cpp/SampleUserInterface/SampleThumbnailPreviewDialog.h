#pragma once

#include "resource.h"
#include "SampleThumbnailPreviewStatic.h"

class CSampleThumbnailPreviewDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleThumbnailPreviewDialog)

public:
  CSampleThumbnailPreviewDialog(CWnd* pParent = NULL);
  virtual ~CSampleThumbnailPreviewDialog();

  // Dialog Data
  enum { IDD = IDD_THUMBNAIL_DIALOG };
  CEdit m_edit;
  CButton m_button;
  CSampleThumbnailPreviewStatic m_preview;

public:
  virtual BOOL OnInitDialog();

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg void OnPreviewButtonClick();
  DECLARE_MESSAGE_MAP()

private:
  ON_wString m_filename;
};
