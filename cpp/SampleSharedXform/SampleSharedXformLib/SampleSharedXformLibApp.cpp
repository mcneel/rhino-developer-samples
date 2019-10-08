#include "stdafx.h"
#include "SampleSharedXformLibApp.h"

// SampleSharedXformLibLibApp class definition

BEGIN_MESSAGE_MAP(SampleSharedXformLibLibApp, CWinApp)
END_MESSAGE_MAP()

// The one and only SampleSharedXformLibLibApp object
SampleSharedXformLibLibApp theApp;

BOOL SampleSharedXformLibLibApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

int SampleSharedXformLibLibApp::ExitInstance()
{
  return CWinApp::ExitInstance();
}
