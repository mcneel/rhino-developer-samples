/////////////////////////////////////////////////////////////////////////////
// PlugInCoreLibApp.h : main header file for the PlugInCoreLib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlugInCoreLibApp
// See PlugInCoreLibApp.cpp for the implementation of this class
//

class CPlugInCoreLibApp : public CWinApp
{
public:
	CPlugInCoreLibApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
