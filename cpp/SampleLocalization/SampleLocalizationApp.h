#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CSampleLocalizationApp : public CWinApp
{
public:
  CSampleLocalizationApp();

  // Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  DECLARE_MESSAGE_MAP()

private:
  CSampleLoadLibrary m_resources;
};
