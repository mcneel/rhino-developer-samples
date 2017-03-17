/////////////////////////////////////////////////////////////////////////////
// SampleCustomUndoPlugIn.h : main header file for the SampleCustomUndo plug-in
//

#pragma once

#include "SampleCustomUndoEventWatcher.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleCustomUndoPlugIn
// See SampleCustomUndoPlugIn.cpp for the implementation of this class
//

class CSampleCustomUndoPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleCustomUndoPlugIn();
  ~CSampleCustomUndoPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Undo helpers
  double Balance() const;
  void SetBalance(double balance);

private:
  ON_wString m_plugin_version;
  
  // TODO: Add additional class information here
  CSampleCustomUndoEventWatcher m_watcher;
  double m_balance;
};

CSampleCustomUndoPlugIn& SampleCustomUndoPlugIn();



