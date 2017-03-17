/////////////////////////////////////////////////////////////////////////////
// WindowsColorDialog.cpp
//

#include "stdafx.h"
#include "WindowsColorDialog.h"

IMPLEMENT_DYNAMIC(CWindowsColorDialog, CColorDialog)

CWindowsColorDialog::CWindowsColorDialog( COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd)
: CColorDialog(clrInit, dwFlags, pParentWnd)
{
  m_strTitle = L"Select Color";
}

CWindowsColorDialog::~CWindowsColorDialog()
{
}

BEGIN_MESSAGE_MAP(CWindowsColorDialog, CColorDialog)
END_MESSAGE_MAP()

BOOL CWindowsColorDialog::OnInitDialog() 
{
  CColorDialog::OnInitDialog();

  SetWindowText( m_strTitle );

  return TRUE; 
}

void CWindowsColorDialog::SetTitle( const wchar_t* pszTitle )
{
  m_strTitle = pszTitle;
}
