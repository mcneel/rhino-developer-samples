
#pragma once

class CSampleRdkAsyncRendererSdkRender : public CRhRdkSdkRender
{
public:
	CSampleRdkAsyncRendererSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn& pPlugin,
	                                 const ON_wString& sCaption, UINT idIcon, bool bPreview);

	int ThreadedRender(void);
	void SetContinueModal(bool b);

	// CRhRdkSdkRender overrides
	virtual BOOL RenderSceneWithNoMeshes(void) override { return true; }
	virtual BOOL RenderEnterModalLoop() override { return true; } // Even though it's not modal, we must return true.
	virtual BOOL RenderContinueModal()  override { return true; } // Even though it's not modal, we must return true.
	virtual BOOL RenderExitModalLoop()  override { return true; } // Even though it's not modal, we must return true.
	virtual BOOL NeedToProcessLightTable() override;
	virtual BOOL NeedToProcessGeometryTable() override;
	virtual BOOL RenderPreCreateWindow() override;
	virtual BOOL IgnoreRhinoObject(const CRhinoObject*) override { return false; }
	virtual BOOL StartRenderingInWindow(CRhinoView* pView, const LPCRECT pRect) override;
	virtual void StartRendering() override;
	virtual void StopRendering() override;
	virtual bool ReuseRenderWindow(void) const override;
	virtual CRhinoSdkRender::RenderReturnCodes Render(const ON_2iSize& sizeRender) override;
	virtual CRhinoSdkRender::RenderReturnCodes RenderWindow(CRhinoView* pView, const LPRECT pRect, bool bInPopupWindow) override;

protected:
	class CAsyncRenderContext* AsyncRC(void) const;
	bool SetUpRender(CRhinoView* pView, bool bQuiet);

private:
	HANDLE m_hRenderThread = NULL;
	bool m_bContinueModal = true;
	bool m_bRenderQuick;
	bool m_bCancel = false;
};
