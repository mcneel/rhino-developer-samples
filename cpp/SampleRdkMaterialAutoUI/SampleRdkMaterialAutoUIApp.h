// SampleRdkMaterialAutoUI.h : main header file for the SampleRdkMaterialAutoUI DLL.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleRdkMaterialAutoUIApp
// See SampleRdkMaterialAutoUIApp.cpp for the implementation of this class
//

class CSampleRdkMaterialAutoUIApp : public CWinApp
{
public:
	CSampleRdkMaterialAutoUIApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
