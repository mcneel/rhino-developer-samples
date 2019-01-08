/////////////////////////////////////////////////////////////////////////////
// rhinoSdkUiSectionSeparator.h

#pragma once

typedef CMap<HWND, HWND, CRect, CRect> CActions;
typedef CMap<HWND, HWND, int, int> CRelated;

class CRhinoUiSectionSeparator : public CRhinoUiStatic
{
  DECLARE_DYNAMIC(CRhinoUiSectionSeparator)

public:
  CRhinoUiSectionSeparator();
  ~CRhinoUiSectionSeparator() = default;

  void SetNextSectionCtrl(const CWnd& wnd);
  void SetNextSectionCtrl(int nId);
  void SetResizeParent(bool bResize);

  void Collapse();
  void Expand();
  bool IsCollapsed();

  enum image_type
  {
    collapse_enabled,
    collapsed_disabled,
    expanded_enabled,
    expanded_disabled,
  };

protected:
  virtual void PreSubclassWindow();
  afx_msg void OnEnable(BOOL bEnable);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  DECLARE_MESSAGE_MAP()

private:
  void UpdateSectionHeight();
  void GetSectionTops(int& nCurrTop, int& nNextTop);
  void MoveControls(CActions& actions);

private:
  CRhinoUiImageList m_imagelist;
  CRect m_rcImage;
  bool m_bCollapsed;
  bool m_bChanging;
  bool m_bResizeParent;
  int m_nNextSectionId;
  int m_nSectionHeight;
  CRelated m_related;
};
