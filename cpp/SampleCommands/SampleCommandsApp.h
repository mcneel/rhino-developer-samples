/////////////////////////////////////////////////////////////////////////////
// SampleCommandsApp.h : main header file for the SampleCommands DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleCommandsApp
// See SampleCommandsApp.cpp for the implementation of this class
//

class CSampleCommandsApp : public CWinApp
{
public:
	CSampleCommandsApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
