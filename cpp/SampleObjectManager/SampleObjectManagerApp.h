/////////////////////////////////////////////////////////////////////////////
// SampleObjectManagerApp.h : main header file for the SampleObjectManager DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleObjectManagerApp
// See SampleObjectManagerApp.cpp for the implementation of this class
//

class CSampleObjectManagerApp : public CWinApp
{
public:
	CSampleObjectManagerApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
