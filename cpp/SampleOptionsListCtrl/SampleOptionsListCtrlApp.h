/////////////////////////////////////////////////////////////////////////////
// SampleOptionsListCtrlApp.h : main header file for the SampleOptionsListCtrl DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleOptionsListCtrlApp
// See SampleOptionsListCtrlApp.cpp for the implementation of this class
//

class CSampleOptionsListCtrlApp : public CWinApp
{
public:
	CSampleOptionsListCtrlApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
