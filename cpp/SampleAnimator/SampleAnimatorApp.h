/////////////////////////////////////////////////////////////////////////////
// SampleAnimatorApp.h : main header file for the SampleAnimator DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleAnimatorApp
// See SampleAnimatorApp.cpp for the implementation of this class
//

class CSampleAnimatorApp : public CWinApp
{
public:
	CSampleAnimatorApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
