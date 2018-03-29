/////////////////////////////////////////////////////////////////////////////
// MarmaladeApp.h : main header file for the Marmalade DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMarmaladeApp
// See MarmaladeApp.cpp for the implementation of this class
//

class CMarmaladeApp : public CWinApp
{
public:
	CMarmaladeApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
