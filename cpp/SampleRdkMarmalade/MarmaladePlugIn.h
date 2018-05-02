
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

	virtual void AddPagesToObjectPropertiesDialog(CRhinoPropertiesPanelPageCollection& collection) override;

	// Required overrides.
	virtual const wchar_t* PlugInName() const override;
	virtual const wchar_t* PlugInVersion() const override;
	virtual GUID PlugInID() const override;
	virtual BOOL OnLoadPlugIn() override;
	virtual void OnUnloadPlugIn() override;
	virtual plugin_load_time PlugInLoadTime() override { return load_plugin_at_startup; }

	// Online help overrides.
	virtual BOOL AddToPlugInHelpMenu() const override;
	virtual BOOL OnDisplayPlugInHelp(HWND hWnd) const override;

	// Render overrides.
	virtual CRhinoCommand::result Render(const CRhinoCommandContext& context, bool bPreview) override;
	virtual CRhinoCommand::result RenderWindow(const CRhinoCommandContext& context, bool bPreview,
                                               CRhinoView* view, const LPRECT rect, bool bInWindow, bool bBlowUp) override;
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

private:
	ON_wString m_plugin_version;
	CMarmaladeEventWatcher m_event_watcher;
	CMarmaladeRdkPlugIn* m_pRdkPlugIn = nullptr;
	CMenu* m_pMenu = nullptr;
	UINT_PTR m_iTimer;
};

CMarmaladePlugIn& MarmaladePlugIn();
