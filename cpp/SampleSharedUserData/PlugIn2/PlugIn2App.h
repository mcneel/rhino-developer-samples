/////////////////////////////////////////////////////////////////////////////
// PlugIn2App.h : main header file for the PlugIn2 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlugIn2App
// See PlugIn2App.cpp for the implementation of this class
//

class CPlugIn2App : public CWinApp
{
public:
	CPlugIn2App();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
