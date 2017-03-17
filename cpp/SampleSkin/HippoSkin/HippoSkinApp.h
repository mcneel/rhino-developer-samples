/////////////////////////////////////////////////////////////////////////////
// HippoSkinApp.h

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHippoSkinApp

class CHippoSkinApp : public CWinApp
{
public:
	CHippoSkinApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd

class CSplashWnd : public CWnd
{
	DECLARE_DYNAMIC( CSplashWnd )

public:
	CSplashWnd();
	virtual ~CSplashWnd();

protected:
  CBitmap m_splash_bitmap;

protected:
	DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnPaint();
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
};
