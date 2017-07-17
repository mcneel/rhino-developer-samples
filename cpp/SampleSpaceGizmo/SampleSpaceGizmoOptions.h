#pragma once

#include "resource.h"
#include "SampleSpaceGizmoSettings.h"

class CSampleSpaceGizmoOptions : public TRhinoOptionsPage<CRhinoDialog>
{
public:
  CSampleSpaceGizmoOptions(CWnd* pParent);
  virtual ~CSampleSpaceGizmoOptions() = default;

  RHINO_PAGE_DECLARE
  RHINO_OPTIONS_PAGE_DECLARE

  // Dialog Data
  enum { IDD = IDD_SPACEGIZMO_OPTIONS };
  CRhinoUiSeparator m_sep1;
  CRhinoUiEditInt	m_RotationScale;
  CRhinoUiEditInt	m_TranslationScale;
  CSampleSpaceGizmoSettings m_settings;

protected:
  virtual BOOL OnInitDialog();
  virtual void PostNcDestroy();
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
  DECLARE_MESSAGE_MAP()
};
