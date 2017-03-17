/////////////////////////////////////////////////////////////////////////////
// SampleObjectUserDataApp.h : main header file for the SampleObjectUserData DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleObjectUserDataApp
// See SampleObjectUserDataApp.cpp for the implementation of this class
//

class CSampleObjectUserDataApp : public CWinApp
{
public:
	CSampleObjectUserDataApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
