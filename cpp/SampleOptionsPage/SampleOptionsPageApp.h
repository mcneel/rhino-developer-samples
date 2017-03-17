/////////////////////////////////////////////////////////////////////////////
// SampleOptionsPageApp.h : main header file for the SampleOptionsPage DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleOptionsPageApp
// See SampleOptionsPageApp.cpp for the implementation of this class
//

class CSampleOptionsPageApp : public CWinApp
{
public:
	CSampleOptionsPageApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
