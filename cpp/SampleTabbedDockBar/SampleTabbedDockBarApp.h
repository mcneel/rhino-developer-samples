/////////////////////////////////////////////////////////////////////////////
// SampleTabbedDockBarApp.h : main header file for the SampleTabbedDockBar DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleTabbedDockBarApp
// See SampleTabbedDockBarApp.cpp for the implementation of this class
//

class CSampleTabbedDockBarApp : public CWinApp
{
public:
	CSampleTabbedDockBarApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
