#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleWithLicensingApp
// See SampleWithLicensingApp.cpp for the implementation of this class
//

class CSampleWithLicensingApp : public CWinApp
{
public:
  CSampleWithLicensingApp();

  // Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();

  DECLARE_MESSAGE_MAP()
};
