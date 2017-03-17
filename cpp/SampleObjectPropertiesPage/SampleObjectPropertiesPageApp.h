/////////////////////////////////////////////////////////////////////////////
// SampleObjectPropertiesPageApp.h : main header file for the SampleObjectPropertiesPage DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleObjectPropertiesPageApp
// See SampleObjectPropertiesPageApp.cpp for the implementation of this class
//

class CSampleObjectPropertiesPageApp : public CWinApp
{
public:
	CSampleObjectPropertiesPageApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
