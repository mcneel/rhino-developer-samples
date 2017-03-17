/////////////////////////////////////////////////////////////////////////////
// SampleDialogsPlugIn.h : main header file for the SampleDialogs plug-in
//

#pragma once

#include "SampleModelessDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleDialogsPlugIn
// See SampleDialogsPlugIn.cpp for the implementation of this class
//

class CSampleDialogsPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleDialogsPlugIn();
  ~CSampleDialogsPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Modeless dialog helpers
  bool IsDlgCreated();
  bool IsDlgVisible();
  bool SetDlgVisible();
  bool SetDlgHidden();
  bool DisplayDlg();
  void DestroyDlg();
  void ZeroDlg();
  void SetDlgPointValue( int item, const ON_3dPoint& pt );

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
  CSampleModelessDialog* m_dialog;
};

CSampleDialogsPlugIn& SampleDialogsPlugIn();



