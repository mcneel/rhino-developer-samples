#include "stdafx.h"
#include "SampleMessageOnlyWnd.h"

IMPLEMENT_DYNAMIC(CSampleMessageOnlyWnd, CWnd)

CSampleMessageOnlyWnd* CSampleMessageOnlyWnd::m_pWnd = nullptr;

CSampleMessageOnlyWnd::CSampleMessageOnlyWnd()
{
}

CSampleMessageOnlyWnd::~CSampleMessageOnlyWnd()
{
}

BEGIN_MESSAGE_MAP(CSampleMessageOnlyWnd, CWnd)
END_MESSAGE_MAP()

bool CSampleMessageOnlyWnd::CreateWnd()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (CSampleMessageOnlyWnd::m_pWnd && ::IsWindow(CSampleMessageOnlyWnd::m_pWnd->GetSafeHwnd()))
    return true;

  if (CSampleMessageOnlyWnd::m_pWnd)
  {
    delete CSampleMessageOnlyWnd::m_pWnd;
    CSampleMessageOnlyWnd::m_pWnd = nullptr;
  }

  m_pWnd = new CSampleMessageOnlyWnd();

  BOOL rc = CSampleMessageOnlyWnd::m_pWnd->CreateEx(
    0,
    AfxRegisterWndClass(0),
    L"Message-Only Window",
    WS_VISIBLE,
    0, 0, 0, 0,
    HWND_MESSAGE,
    0
  );

  if (!rc)
  {
    delete CSampleMessageOnlyWnd::m_pWnd;
    CSampleMessageOnlyWnd::m_pWnd = nullptr;
    return false;
  }

  return true;
}

void CSampleMessageOnlyWnd::DestroyWnd()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (CSampleMessageOnlyWnd::m_pWnd)
  {
    if (::IsWindow(CSampleMessageOnlyWnd::m_pWnd->m_hWnd))
      CSampleMessageOnlyWnd::m_pWnd->DestroyWindow();

    delete CSampleMessageOnlyWnd::m_pWnd;
    CSampleMessageOnlyWnd::m_pWnd = nullptr;
  }
}

LRESULT CSampleMessageOnlyWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  // TODO...
  return CWnd::WindowProc(message, wParam, lParam);
}
