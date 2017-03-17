/////////////////////////////////////////////////////////////////////////////
// SampleCustomGripsApp.h : main header file for the SampleCustomGrips DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleCustomGripsApp
// See SampleCustomGripsApp.cpp for the implementation of this class
//

class CSampleCustomGripsApp : public CWinApp
{
public:
	CSampleCustomGripsApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
