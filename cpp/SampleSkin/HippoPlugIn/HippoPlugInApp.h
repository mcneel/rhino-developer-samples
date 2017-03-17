/////////////////////////////////////////////////////////////////////////////
// HippoPlugInApp.h : main header file for the HippoPlugIn DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHippoPlugInApp
// See HippoPlugInApp.cpp for the implementation of this class
//

class CHippoPlugInApp : public CWinApp
{
public:
	CHippoPlugInApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
