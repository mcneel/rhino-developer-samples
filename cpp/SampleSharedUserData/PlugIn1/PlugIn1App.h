/////////////////////////////////////////////////////////////////////////////
// PlugIn1App.h : main header file for the PlugIn1 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlugIn1App
// See PlugIn1App.cpp for the implementation of this class
//

class CPlugIn1App : public CWinApp
{
public:
	CPlugIn1App();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
