/////////////////////////////////////////////////////////////////////////////
// WindowsColorDialog.h
//

#pragma once

class CWindowsColorDialog : public CColorDialog
{
  DECLARE_DYNAMIC(CWindowsColorDialog)

public:
  CWindowsColorDialog( COLORREF clrInit = 0, DWORD dwFlags = 0, CWnd* pParentWnd = NULL );
  virtual ~CWindowsColorDialog();

  void SetTitle( const wchar_t* pszTitle );

protected:
  virtual BOOL OnInitDialog();
  DECLARE_MESSAGE_MAP()

protected:
  CString m_strTitle;
};
