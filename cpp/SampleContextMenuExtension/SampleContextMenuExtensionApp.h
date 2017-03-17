/////////////////////////////////////////////////////////////////////////////
// SampleContextMenuExtensionApp.h : main header file for the SampleContextMenuExtension DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleContextMenuExtensionApp
// See SampleContextMenuExtensionApp.cpp for the implementation of this class
//

class CSampleContextMenuExtensionApp : public CWinApp
{
public:
	CSampleContextMenuExtensionApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
