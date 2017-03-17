#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CSampleSkinApp : public CWinApp
{
public:
  CSampleSkinApp();

  // Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  DECLARE_MESSAGE_MAP()
};

// CSplashWnd
// See SampleSkinApp.cpp for the implementation of this class
//

class CSplashWnd : public CWnd
{
  DECLARE_DYNAMIC(CSplashWnd)

public:
  CSplashWnd();
  virtual ~CSplashWnd();

protected:
  CBitmap m_splash_bitmap;

protected:
  DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnPaint();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
