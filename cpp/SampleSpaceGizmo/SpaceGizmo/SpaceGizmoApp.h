/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoApp.h

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

// Description:
//   Space Gizmo CWinApp

class CSpaceGizmoApp : public CWinApp
{
public:
	CSpaceGizmoApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
