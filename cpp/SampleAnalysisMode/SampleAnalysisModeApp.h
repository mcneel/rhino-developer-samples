/////////////////////////////////////////////////////////////////////////////
// SampleAnalysisModeApp.h : main header file for the SampleAnalysisMode DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleAnalysisModeApp
// See SampleAnalysisModeApp.cpp for the implementation of this class
//

class CSampleAnalysisModeApp : public CWinApp
{
public:
	CSampleAnalysisModeApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
