#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h" // main symbols

// SampleSharedXformLibLibApp class declaration

class SampleSharedXformLibLibApp : public CWinApp
{
public:
	SampleSharedXformLibLibApp() = default;
	BOOL InitInstance() override;
	int ExitInstance() override;
	DECLARE_MESSAGE_MAP()
};
