#pragma once

class CSampleSpaceGizmoWnd : public CWnd
{
  DECLARE_DYNAMIC(CSampleSpaceGizmoWnd)

public:
  CSampleSpaceGizmoWnd();
  virtual ~CSampleSpaceGizmoWnd();

  BOOL InitializeSpaceGizmo();

protected:
  LRESULT OnSpaceGizmo(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

private:
  SiHdl m_siHdl;
  static UINT WM_SPACEGIZMO;
};


