// SampleRdkContentIOPlugIn.h : main header file for the SampleRdkContentIOPlugIn DLL.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleRdkContentIOPlugInRhinoPlugInApp
// See SampleRdkContentIOPlugInApp.cpp for the implementation of this class
//

class CSampleRdkContentIOPlugInRhinoPlugInApp : public CWinApp
{
public:
	CSampleRdkContentIOPlugInRhinoPlugInApp();

// Overrides
public:
	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;
	DECLARE_MESSAGE_MAP()
};
