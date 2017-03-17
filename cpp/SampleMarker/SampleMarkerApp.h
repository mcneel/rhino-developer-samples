#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleMarkerApp
// See SampleMarkerApp.cpp for the implementation of this class
//

class CSampleMarkerApp : public CWinApp
{
public:
  CSampleMarkerApp();

  // Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  DECLARE_MESSAGE_MAP()
};
