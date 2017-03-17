#pragma once

#include "SampleModelessDialog.h"
#include "SampleLayerContextMenuExtension.h"

class CSampleUserInterfacePlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleUserInterfacePlugIn();
  ~CSampleUserInterfacePlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  // Additional overrides
  CRhinoPlugIn::plugin_load_time PlugInLoadTime();
  void AddPagesToOptionsDialog(HWND hWnd, ON_SimpleArray<class CRhinoOptionsDialogPage*>& pages);
  void AddPagesToDocumentPropertiesDialog(CRhinoDoc& doc, HWND hWnd, ON_SimpleArray<class CRhinoOptionsDialogPage*>& pages);
  void AddPagesToObjectPropertiesDialog(ON_SimpleArray<class CRhinoPropertiesDialogPage*>& pages);
  void OnInitPlugInMenuPopups(WPARAM wParam, LPARAM lParam);
  BOOL OnPlugInMenuCommand(WPARAM wParam);

  bool ScriptMode() const;
  void SetScriptMode(bool bSet);

  // Modeless dialog helpers
  bool IsDlgCreated();
  bool IsDlgVisible();
  bool SetDlgVisible();
  bool SetDlgHidden();
  bool DisplayDlg();
  void DestroyDlg();
  void ZeroDlg();
  void SetDlgPointValue(int item, const ON_3dPoint& pt);

  // Plug-in specific menu helpers
  BOOL IsSampleMenuVisible() const;
  BOOL ShowSampleMenu();
  BOOL HideSampleMenu();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  CSampleModelessDialog* m_dialog;
  CSampleLayerContextMenuExtension* m_extension_menu;
  CMenu m_menu;
  bool m_bScriptMode;
};

CSampleUserInterfacePlugIn& SampleUserInterfacePlugIn();



