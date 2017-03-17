#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CSampleSharedUserData2App : public CWinApp
{
public:
  CSampleSharedUserData2App();

  // Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  DECLARE_MESSAGE_MAP()
};
