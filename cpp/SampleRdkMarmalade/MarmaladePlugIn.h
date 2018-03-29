
#pragma once

#include "MarmaladeEventWatcher.h"

class CMarmaladeRdkPlugIn;
class CMarmaladeNonModalOptionsDlg;

class CMarmaladePlugIn : public CRhinoRenderPlugIn
{
public:
	CMarmaladePlugIn();
	virtual ~CMarmaladePlugIn();

	static GUID ID(void);

	// Required overrides.
	virtual const wchar_t* PlugInName() const;
	virtual const wchar_t* PlugInVersion() const;
	virtual GUID PlugInID() const;
	virtual BOOL OnLoadPlugIn();
	virtual void OnUnloadPlugIn();

	// Online help overrides.
	virtual BOOL AddToPlugInHelpMenu() const;
	virtual BOOL OnDisplayPlugInHelp( HWND hWnd ) const;

	// Render overrides.
	virtual CRhinoCommand::result Render(const CRhinoCommandContext& context, bool bPreview);
	virtual CRhinoCommand::result RenderWindow(const CRhinoCommandContext& context, bool bPreview, CRhinoView* view, const LPRECT rect, bool bInWindow);

	// Menu.
	virtual void AddMarmaladeMenu(void);
	virtual void RemoveMarmaladeMenu(void);
	virtual BOOL OnPlugInMenuCommand(WPARAM wParam);
	virtual void UpdateMarmaladeMenuState(void);

	// Render methods
	bool SceneChanged(void) const;
	bool LightingChanged(void) const;
	void SetSceneChanged(bool bChanged);
	void SetLightingChanged(bool bChanged);

	// Misc.
	CMarmaladeRdkPlugIn& RdkPlugIn(void) { return *m_pRdkPlugIn; }

	bool IsMarmaladeCurrentRenderer(void) const;

	// Non-modal options dialog. How about having a handler object for this stuff?
	void ShowNonModalOptionsDialog(bool bShow);
	void ToggleNonModalOptionsDialog(void);
	void EnableNonModalOptionsDialog(bool bEnable) const;
	bool IsNonModalOptionsDialogVisible(void) const;
	bool IsNonModalOptionsDialogMinimized(void) const;

	virtual plugin_load_time PlugInLoadTime() { return load_plugin_at_startup; }

private:
	ON_wString m_plugin_version;
	CMarmaladeEventWatcher m_event_watcher;
	CMarmaladeRdkPlugIn* m_pRdkPlugIn;
	CMenu* m_pMenu;
	UINT_PTR m_iTimer;
	CMarmaladeNonModalOptionsDlg* m_pNonModalOptionsDialog;
};

CMarmaladePlugIn& MarmaladePlugIn();
