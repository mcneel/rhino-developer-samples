/////////////////////////////////////////////////////////////////////////////
// SampleUserTextApp.h : main header file for the SampleUserText DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleUserTextApp
// See SampleUserTextApp.cpp for the implementation of this class
//

class CSampleUserTextApp : public CWinApp
{
public:
	CSampleUserTextApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
