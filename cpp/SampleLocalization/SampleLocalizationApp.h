/////////////////////////////////////////////////////////////////////////////
// SampleLocalizationApp.h : main header file for the SampleLocalization DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleLocalizationApp
// See SampleLocalizationApp.cpp for the implementation of this class
//

class CSampleLocalizationApp : public CWinApp
{
public:
	CSampleLocalizationApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()

private:
  CRhinoLoadLibrary m_resources;
};
