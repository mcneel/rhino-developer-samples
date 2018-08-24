
#pragma once

#include "SampleRdkEffectsPanelPlugIn.h"
#include "Resource.h"

class CSampleRdkEffectsPanelDialog : public CRhRdkResizingDialog
{
public:
	CSampleRdkEffectsPanelDialog(IRhRdkRenderWindow& rw);

	bool CreateWnd(CWnd* pParent);

private:
	enum { IDD = IDD_TAB_CHILD }; // This is the dialog containing the controls you would want to display.
	CRhRdkSdkRealNumberEdit m_edit;
	CButton m_check;
	CRhRdkColorButton m_button;

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;

	void UpdateRendering(void);
	void EnableDisableControls(void);

protected:
	afx_msg void OnEdit();
	afx_msg void OnCheck();
	afx_msg LRESULT OnColorChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	IRhRdkRenderWindow& m_RW;
};

/** \class CSampleRdkEffectsPanelTab

	This is the 'tab' which is created by your tab factory.
	This is not a window, but it manages the underlying dialog
	which in this case is CSampleRdkEffectsPanelDialog (above).

*/
class CSampleRdkEffectsPanelTab : public CRhinoUiDockBarTab
{
public:
	CSampleRdkEffectsPanelTab(IRhRdkRenderWindow& rw) : m_RW(rw) { }
	virtual ~CSampleRdkEffectsPanelTab() { ASSERT(nullptr == m_pDialog); }

	virtual bool CreateWnd() override;
	virtual void DeleteThis(void) override { delete this; }
	virtual void MoveWnd(const ON_4iRect& rect) override;
	virtual void ShowWnd(UINT uState) override;
	virtual void DestroyWnd(void) override;
	virtual bool IsCreated(void) const override;
	virtual void SwitchDockBar() override;
	virtual UUID TabId(void) const override;
	virtual HICON Icon(const ON_2iSize& sizeInPixels) const override;
	virtual ITabbedDockBarEventWatcher* TabbedDockBarEventWatcher(void) const override { return nullptr; }
	virtual ON_wString Caption(void) const override { return L"SampleRdkEffectsPanel"; }
	virtual void DoHelp(void) const override { }

private:
	HWND PanelHost(void) const;

private:
	IRhRdkRenderWindow& m_RW;
	CSampleRdkEffectsPanelDialog* m_pDialog = nullptr;
};

class CSampleRdkEffectsPanelTabFactory : public CRhRdkRenderFrameTabFactoryEx
{
public:
	virtual CRhinoUiDockBarTab* NewTab(IRhRdkRenderWindow& rw) const override;
	virtual UUID PlugInId(void) const override { return ::SampleRdkEffectsPanelPlugIn().PlugInID(); }
	virtual UUID RenderEngineId(void) const override { return ::SampleRdkEffectsPanelPlugIn().PlugInID(); }
	virtual UUID TabId(void) const override;

	// This function is for future use.
	virtual class CRhinoUiDockBarTab* NewTabEx(CRhRdkControllerPtr con, HWND parent) const override { return nullptr; }
};
