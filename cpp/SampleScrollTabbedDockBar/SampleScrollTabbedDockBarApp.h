/////////////////////////////////////////////////////////////////////////////
// SampleScrollTabbedDockBarApp.h : main header file for the SampleScrollTabbedDockBar DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleScrollTabbedDockBarApp
// See SampleScrollTabbedDockBarApp.cpp for the implementation of this class
//

class CSampleScrollTabbedDockBarApp : public CWinApp
{
public:
	CSampleScrollTabbedDockBarApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
