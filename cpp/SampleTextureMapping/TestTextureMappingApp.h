/////////////////////////////////////////////////////////////////////////////
// TestTextureMappingApp.h : main header file for the TestTextureMapping DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestTextureMappingApp
// See TestTextureMappingApp.cpp for the implementation of this class
//

class CTestTextureMappingApp : public CWinApp
{
public:
	CTestTextureMappingApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
