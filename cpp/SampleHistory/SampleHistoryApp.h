/////////////////////////////////////////////////////////////////////////////
// SampleHistoryApp.h : main header file for the SampleHistory DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleHistoryApp
// See SampleHistoryApp.cpp for the implementation of this class
//

class CSampleHistoryApp : public CWinApp
{
public:
	CSampleHistoryApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
