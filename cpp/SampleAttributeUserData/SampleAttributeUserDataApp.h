/////////////////////////////////////////////////////////////////////////////
// SampleAttributeUserDataApp.h : main header file for the SampleAttributeUserData DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleAttributeUserDataApp
// See SampleAttributeUserDataApp.cpp for the implementation of this class
//

class CSampleAttributeUserDataApp : public CWinApp
{
public:
	CSampleAttributeUserDataApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
