/////////////////////////////////////////////////////////////////////////////
// SampleExportMeshApp.h : main header file for the SampleExportMesh DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleExportMeshApp
// See SampleExportMeshApp.cpp for the implementation of this class
//

class CSampleExportMeshApp : public CWinApp
{
public:
	CSampleExportMeshApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
