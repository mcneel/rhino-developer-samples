// ColorSwatch.cpp : implementation file
//

#include "stdafx.h"
#include "ColorSwatch.h"


// CColorSwatch

IMPLEMENT_DYNAMIC(CColorSwatch, CStatic)

CColorSwatch::CColorSwatch()
: m_color(::GetSysColor(COLOR_BTNFACE))
{

}

CColorSwatch::~CColorSwatch()
{
}

COLORREF CColorSwatch::Color() const { return m_color; }

void CColorSwatch::SetColor(COLORREF color)
{
  if (color == m_color)
    return;
  m_color = color;
  if (::IsWindow(GetSafeHwnd()))
    Invalidate();
}

void CColorSwatch::Draw(CDC& dc)
{
  CRect r;
  GetClientRect(r);
  dc.FillSolidRect(r, IsWindowEnabled() ? Color() : ::GetSysColor(COLOR_BTNFACE));
}

BEGIN_MESSAGE_MAP(CColorSwatch, CStatic)
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
END_MESSAGE_MAP()



// CColorSwatch message handlers

void CColorSwatch::OnPaint()
{
  CPaintDC dc(this);
  Draw(dc);
}

BOOL CColorSwatch::OnEraseBkgnd(CDC* pDC)
{
  if (pDC)
    Draw(*pDC);
  return true;
}
