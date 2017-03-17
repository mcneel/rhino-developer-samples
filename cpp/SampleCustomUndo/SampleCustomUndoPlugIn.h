#pragma once

#include "SampleCustomUndoEventWatcher.h"

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

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

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



