/////////////////////////////////////////////////////////////////////////////
// SampleEventWatcherApp.h : main header file for the SampleEventWatcher DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleEventWatcherApp
// See SampleEventWatcherApp.cpp for the implementation of this class
//

class CSampleEventWatcherApp : public CWinApp
{
public:
	CSampleEventWatcherApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
