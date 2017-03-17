/////////////////////////////////////////////////////////////////////////////
// BlockWalkApp.h

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBlockWalkApp
// See BlockWalkApp.cpp for the implementation of this class

class CBlockWalkApp : public CWinApp
{
public:
	CBlockWalkApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
