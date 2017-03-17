/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoOptions.h

#pragma once

#include "resource.h"
#include "SpaceGizmoSettings.h"

// Description:
//   Space Gizmo options dialog page that appears in Rhino's Options
//   dialog when the plug-in is loaded

class CSpaceGizmoOptions : public CRhinoOptionsDialogPage
{
	DECLARE_DYNAMIC( CSpaceGizmoOptions )

public:
	CSpaceGizmoOptions( CWnd* pParent );
	virtual ~CSpaceGizmoOptions();

  // Dialog Data
	enum { IDD = IDD_SPACEGIZMO_OPTIONS };
  CRhinoUiSeparator m_sep1;
	CRhinoUiEditInt	m_RotationScale;
	CRhinoUiEditInt	m_TranslationScale;
	CSpaceGizmoSettings m_settings;

  // Overrides
  int OnApply();
  void OnCancel();
  const wchar_t* EnglishPageTitle();
  const wchar_t* LocalPageTitle();
  void RhinoDeleteThisPage();
  CRhinoCommand::result RunScript( CRhinoDoc& doc );

protected:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void DoDataExchange( CDataExchange* pDX );
	afx_msg BOOL OnHelpInfo( HELPINFO* pHelpInfo );
	DECLARE_MESSAGE_MAP()
};
