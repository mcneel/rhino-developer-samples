/////////////////////////////////////////////////////////////////////////////
// SampleDockbarApp.h
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

// CSampleDockbarApp

class CSampleDockbarApp : public CWinApp
{
public:
	CSampleDockbarApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
