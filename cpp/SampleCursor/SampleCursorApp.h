/////////////////////////////////////////////////////////////////////////////
// SampleCursorApp.h : main header file for the SampleCursor DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleCursorApp
// See SampleCursorApp.cpp for the implementation of this class
//

class CSampleCursorApp : public CWinApp
{
public:
	CSampleCursorApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
