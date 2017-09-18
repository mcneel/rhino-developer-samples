// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "ColorButton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)

CColorButton::CColorButton()
: m_color(::GetSysColor(COLOR_BTNFACE))
{

}

CColorButton::~CColorButton()
{
}

COLORREF CColorButton::Color() const { return m_color; }

void CColorButton::SetColor(COLORREF color)
{
  if (color == m_color)
    return;
  m_color = color;
  if (::IsWindow(GetSafeHwnd()))
    Invalidate();
}

void CColorButton::Draw(CDC& dc)
{
  CRect r;
  GetClientRect(r);
  dc.FillSolidRect(r, IsWindowEnabled() ? Color() : ::GetSysColor(COLOR_BTNFACE));
}

BOOL CColorButton::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  if( WM_COMMAND == message && BN_CLICKED == HIWORD(wParam))
    ButtonClicked();
  return CButton::OnChildNotify(message, wParam, lParam, pResult);
}

void CColorButton::ButtonClicked()
{
	CWnd *pParent = GetParent();

  ON_Color on_color(Color());

  if (::RhinoColorDialog(pParent ? pParent->GetSafeHwnd() : RhinoApp().MainWnd(), on_color, false))
  	SetColor(on_color);
}

BEGIN_MESSAGE_MAP(CColorButton, CButton)
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
END_MESSAGE_MAP()

// CColorButton message handlers

void CColorButton::OnPaint()
{
  CPaintDC dc(this);
  Draw(dc);
}

BOOL CColorButton::OnEraseBkgnd(CDC* pDC)
{
  if (pDC)
    Draw(*pDC);
  return true;
}
