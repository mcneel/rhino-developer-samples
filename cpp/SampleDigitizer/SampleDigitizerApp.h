/////////////////////////////////////////////////////////////////////////////
// SampleDigitizerApp.h : main header file for the SampleDigitizer DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleDigitizerApp
// See SampleDigitizerApp.cpp for the implementation of this class
//

class CSampleDigitizerApp : public CWinApp
{
public:
	CSampleDigitizerApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
