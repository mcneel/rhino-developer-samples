#pragma once

#include "SampleModelessDialog.h"
#include "SampleLayerContextMenuExtension.h"

class CSampleUserInterfacePlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleUserInterfacePlugIn();
  ~CSampleUserInterfacePlugIn();

  // Required overrides
  const wchar_t* PlugInName() const override;
  const wchar_t* PlugInVersion() const override;
  GUID PlugInID() const override;
  BOOL OnLoadPlugIn() override;
  void OnUnloadPlugIn() override;

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const override;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const override;

  // Additional overrides
  CRhinoPlugIn::plugin_load_time PlugInLoadTime() override;
  void AddPagesToOptionsDialog(CRhinoOptionsPageCollection& collection) override;
  void AddPagesToDocumentPropertiesDialog(CRhinoOptionsPageCollection& collection) override;
  void AddPagesToObjectPropertiesDialog(CRhinoPropertiesPanelPageCollection& collection);
  void OnInitPlugInMenuPopups(WPARAM wParam, LPARAM lParam) override;
  BOOL OnPlugInMenuCommand(WPARAM wParam) override;

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

  void RunCommandScript(const wchar_t* script);

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  CSampleModelessDialog* m_dialog;
  CSampleLayerContextMenuExtension* m_extension_menu;
  CMenu m_menu;
  bool m_bScriptMode;
};

CSampleUserInterfacePlugIn& SampleUserInterfacePlugIn();



