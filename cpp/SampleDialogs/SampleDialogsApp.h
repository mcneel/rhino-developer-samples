/////////////////////////////////////////////////////////////////////////////
// SampleDialogsApp.h : main header file for the SampleDialogs DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleDialogsApp
// See SampleDialogsApp.cpp for the implementation of this class
//

class CSampleDialogsApp : public CWinApp
{
public:
	CSampleDialogsApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
