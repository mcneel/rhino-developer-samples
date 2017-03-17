/////////////////////////////////////////////////////////////////////////////
// SampleRhinoApp.h : main header file for the SampleRhino DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleRhinoApp
// See SampleRhinoApp.cpp for the implementation of this class
//

class CSampleRhinoApp : public CWinApp
{
public:
	CSampleRhinoApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
