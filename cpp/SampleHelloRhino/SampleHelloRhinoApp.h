/////////////////////////////////////////////////////////////////////////////
// SampleHelloRhinoApp.h : main header file for the SampleHelloRhino DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleHelloRhinoApp
// See SampleHelloRhinoApp.cpp for the implementation of this class
//

class CSampleHelloRhinoApp : public CWinApp
{
public:
	CSampleHelloRhinoApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
