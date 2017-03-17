/////////////////////////////////////////////////////////////////////////////
// SampleDocumentUserDataApp.h : main header file for the SampleDocumentUserData DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleDocumentUserDataApp
// See SampleDocumentUserDataApp.cpp for the implementation of this class
//

class CSampleDocumentUserDataApp : public CWinApp
{
public:
	CSampleDocumentUserDataApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
