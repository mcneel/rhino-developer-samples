/////////////////////////////////////////////////////////////////////////////
// SampleImportPointsApp.h : main header file for the SampleImportPoints DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleImportPointsApp
// See SampleImportPointsApp.cpp for the implementation of this class
//

class CSampleImportPointsApp : public CWinApp
{
public:
	CSampleImportPointsApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
