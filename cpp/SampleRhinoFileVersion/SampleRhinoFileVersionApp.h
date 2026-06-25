#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CSampleRhinoFileVersionApp : public CWinApp
{
public:
  CSampleRhinoFileVersionApp();

  // Overrides
public:
  virtual BOOL InitInstance() override;
  virtual int ExitInstance() override;
  DECLARE_MESSAGE_MAP()
};
