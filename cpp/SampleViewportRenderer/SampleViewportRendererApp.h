/////////////////////////////////////////////////////////////////////////////
// SampleViewportRendererApp.h 
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

class CSampleViewportRendererApp : public CWinApp
{
public:
	CSampleViewportRendererApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
