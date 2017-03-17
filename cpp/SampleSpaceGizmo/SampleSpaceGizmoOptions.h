#pragma once

#include "resource.h"
#include "SampleSpaceGizmoSettings.h"

class CSampleSpaceGizmoOptions : public CRhinoOptionsDialogPage
{
  DECLARE_DYNAMIC(CSampleSpaceGizmoOptions)

public:
  CSampleSpaceGizmoOptions(CWnd* pParent);
  virtual ~CSampleSpaceGizmoOptions();

  // Dialog Data
  enum { IDD = IDD_SPACEGIZMO_OPTIONS };
  CRhinoUiSeparator m_sep1;
  CRhinoUiEditInt	m_RotationScale;
  CRhinoUiEditInt	m_TranslationScale;
  CSampleSpaceGizmoSettings m_settings;

  // Overrides
  int OnApply();
  void OnCancel();
  const wchar_t* EnglishPageTitle();
  const wchar_t* LocalPageTitle();
  void RhinoDeleteThisPage();
  CRhinoCommand::result RunScript(const unsigned int rhino_doc_sn);

protected:
  virtual BOOL OnInitDialog();
  virtual void PostNcDestroy();
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
  DECLARE_MESSAGE_MAP()
};
