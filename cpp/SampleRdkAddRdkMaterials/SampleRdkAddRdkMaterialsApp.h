
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleRdkAddRdkMaterialsApp
// See SampleRdkAddRdkMaterialsApp.cpp for the implementation of this class
//

class CSampleRdkAddRdkMaterialsApp : public CWinApp
{
public:
	CSampleRdkAddRdkMaterialsApp();

public:
	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;
	DECLARE_MESSAGE_MAP()
};
