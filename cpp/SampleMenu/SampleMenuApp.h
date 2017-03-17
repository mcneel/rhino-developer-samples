/////////////////////////////////////////////////////////////////////////////
// SampleMenuApp.h : main header file for the SampleMenu DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleMenuApp
// See SampleMenuApp.cpp for the implementation of this class
//

class CSampleMenuApp : public CWinApp
{
public:
	CSampleMenuApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
