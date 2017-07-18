#pragma once

#include "resource.h"
#include "SampleSpaceGizmoSettings.h"

class CSampleSpaceGizmoOptions : public TRhinoOptionsPage<CRhinoDialog>
{
public:
  CSampleSpaceGizmoOptions();
  ~CSampleSpaceGizmoOptions() = default;

  RHINO_PAGE_DECLARE
  RHINO_OPTIONS_PAGE_DECLARE

  // Dialog Data
  enum { IDD = IDD_SPACEGIZMO_OPTIONS };
  CRhinoUiSeparator m_sep1;
  CRhinoUiEditInt	m_RotationScale;
  CRhinoUiEditInt	m_TranslationScale;
  CSampleSpaceGizmoSettings m_settings;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
  DECLARE_MESSAGE_MAP()
};
