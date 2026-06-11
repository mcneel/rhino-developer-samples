#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CSampleExportMeshApp

class CSampleExportMeshApp : public CWinApp
{
public:
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  // Only standard MFC DLL instance construction belongs here. 
  // All other significant initialization should take place in
  // CSampleExportMeshPlugIn::OnLoadPlugIn().
  CSampleExportMeshApp() = default;

public:
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  // Only standard MFC DLL instance initialization belongs here. 
  // All other significant initialization should take place in
  // CSampleExportMeshApp::OnLoadPlugIn().
  virtual BOOL InitInstance() override;
  
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  // Only standard MFC DLL instance clean up belongs here. 
  // All other significant cleanup should take place in either
  // CSampleExportMeshApp::OnSaveAllSettings() or
  // CSampleExportMeshApp::OnUnloadPlugIn().  
  virtual int ExitInstance() override;
  
  DECLARE_MESSAGE_MAP()
};
