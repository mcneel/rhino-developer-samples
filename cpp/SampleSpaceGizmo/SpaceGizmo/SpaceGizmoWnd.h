/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoWnd.h

#pragma once

// Description:
//   Space Gizmo message-only window

class CSpaceGizmoWnd : public CWnd
{
	DECLARE_DYNAMIC( CSpaceGizmoWnd )

public:
	CSpaceGizmoWnd();
	virtual ~CSpaceGizmoWnd();

  BOOL InitializeSpaceGizmo();

protected:
  LRESULT OnSpaceGizmo( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
  SiHdl m_siHdl;
  static UINT WM_SPACEGIZMO;
};


