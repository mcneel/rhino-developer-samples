/////////////////////////////////////////////////////////////////////////////
// SamplePropertiesPagesApp.h : main header file for the SamplePropertiesPages DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSamplePropertiesPagesApp
// See SamplePropertiesPagesApp.cpp for the implementation of this class
//

class CSamplePropertiesPagesApp : public CWinApp
{
public:
	CSamplePropertiesPagesApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
