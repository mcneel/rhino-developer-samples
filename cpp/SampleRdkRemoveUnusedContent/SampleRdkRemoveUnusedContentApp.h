
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleRdkRemoveUnusedContentApp
// See SampleRdkRemoveUnusedContentApp.cpp for the implementation of this class
//

class CSampleRdkRemoveUnusedContentApp : public CWinApp
{
public:
	CSampleRdkRemoveUnusedContentApp();

public:
	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;
	DECLARE_MESSAGE_MAP()
};
