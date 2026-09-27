
#pragma once

#include <atomic>
#include <thread>

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
	virtual BOOL32 RenderSceneWithNoMeshes(void) override { return true; }
	virtual BOOL32 RenderEnterModalLoop() override { return true; }
	virtual BOOL32 RenderContinueModal()  override { return m_bContinueModal; }
	virtual BOOL32 RenderExitModalLoop()  override { return true; }
	virtual BOOL32 NeedToProcessLightTable() override;
	virtual BOOL32 NeedToProcessGeometryTable() override;
	virtual BOOL32 RenderPreCreateWindow() override;
	virtual BOOL32 IgnoreRhinoObject(const CRhinoObject*) override { return false; }
	virtual BOOL32 StartRenderingInWindow(CRhinoView* pView, const LPCRECT pRect) override;
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
	ON_4iRect m_RectRender;
	ON_4iRect m_Region;
	std::thread m_RenderThread;
	ON_3dmRenderSettings m_RenderSettings;
	IRhRdkRenderWindow::IChannel* m_pChanRGBA = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanZ = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalX = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalY = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalZ = nullptr;
	bool m_bPreview = false;
	bool m_bContinueModal = true;

	// Written by the main thread in StopRendering() and read by the render
	// thread, so it has to be atomic.  volatile is not a threading construct.
	std::atomic<bool> m_bCancel{false};
};
