/////////////////////////////////////////////////////////////////////////////
// SampleClippingPlanesApp.h : main header file for the SampleClippingPlanes DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleClippingPlanesApp
// See SampleClippingPlanesApp.cpp for the implementation of this class
//

class CSampleClippingPlanesApp : public CWinApp
{
public:
	CSampleClippingPlanesApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
