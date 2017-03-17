/////////////////////////////////////////////////////////////////////////////
// SampleCustomUndoApp.h : main header file for the SampleCustomUndo DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleCustomUndoApp
// See SampleCustomUndoApp.cpp for the implementation of this class
//

class CSampleCustomUndoApp : public CWinApp
{
public:
	CSampleCustomUndoApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
