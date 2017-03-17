/////////////////////////////////////////////////////////////////////////////
// SampleTabDialogApp.h : main header file for the SampleTabDialog DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleTabDialogApp
// See SampleTabDialogApp.cpp for the implementation of this class
//

class CSampleTabDialogApp : public CWinApp
{
public:
	CSampleTabDialogApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
