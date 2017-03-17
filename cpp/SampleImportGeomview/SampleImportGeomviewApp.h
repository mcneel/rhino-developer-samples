/////////////////////////////////////////////////////////////////////////////
// SampleImportGeomviewApp.h : main header file for the SampleImportGeomview DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleImportGeomviewApp
// See SampleImportGeomviewApp.cpp for the implementation of this class
//

class CSampleImportGeomviewApp : public CWinApp
{
public:
	CSampleImportGeomviewApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
