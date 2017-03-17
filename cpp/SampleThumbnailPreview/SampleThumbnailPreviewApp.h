/////////////////////////////////////////////////////////////////////////////
// SampleThumbnailPreviewApp.h : main header file for the SampleThumbnailPreview DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleThumbnailPreviewApp
// See SampleThumbnailPreviewApp.cpp for the implementation of this class
//

class CSampleThumbnailPreviewApp : public CWinApp
{
public:
	CSampleThumbnailPreviewApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
