
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleRdkMaterialWithChildrenApp
// See SampleRdkMaterialWithChildrenApp.cpp for the implementation of this class
//

class CSampleRdkMaterialWithChildrenApp : public CWinApp
{
public:
	CSampleRdkMaterialWithChildrenApp();

public:
	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;
	DECLARE_MESSAGE_MAP()
};
