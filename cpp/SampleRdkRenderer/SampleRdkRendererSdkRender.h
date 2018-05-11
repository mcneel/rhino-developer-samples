
#pragma once

class CSampleRdkRendererSdkRender : public CRhRdkSdkRender
{
public:
	CSampleRdkRendererSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn& pPlugin,
	                                 const ON_wString& sCaption, UINT idIcon, bool bPreview);

	virtual ~CSampleRdkRendererSdkRender();

	int ThreadedRender(void);
	void SetContinueModal(bool b);

public:
	// CRhRdkSdkRender overrides.
	virtual BOOL RenderSceneWithNoMeshes(void) override { return true; }
	virtual BOOL RenderEnterModalLoop() override { return true; }
	virtual BOOL RenderContinueModal()  override { return m_bContinueModal; }
	virtual BOOL RenderExitModalLoop()  override { return true; }
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
	bool RenderCore(void);

private:
	CRect m_RectRender;
	CRect m_Region;
	HANDLE m_hRenderThread = NULL;
	ON_3dmRenderSettings m_RenderSettings;
	IRhRdkRenderWindow::IChannel* m_pChanRGBA = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanZ = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalX = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalY = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalZ = nullptr;
	bool m_bPreview = false;
	bool m_bContinueModal = true;
	volatile bool m_bCancel = false;
};
