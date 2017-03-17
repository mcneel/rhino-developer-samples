/////////////////////////////////////////////////////////////////////////////
// SampleColorPickerApp.h : main header file for the SampleColorPicker DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleColorPickerApp
// See SampleColorPickerApp.cpp for the implementation of this class
//

class CSampleColorPickerApp : public CWinApp
{
public:
	CSampleColorPickerApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
