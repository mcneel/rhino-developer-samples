/////////////////////////////////////////////////////////////////////////////
// SampleDisplayConduitsApp.h : main header file for the SampleDisplayConduits DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleDisplayConduitsApp
// See SampleDisplayConduitsApp.cpp for the implementation of this class
//

class CSampleDisplayConduitsApp : public CWinApp
{
public:
	CSampleDisplayConduitsApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
