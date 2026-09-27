
#pragma once

class CSampleRdkAsyncRendererSdkRender : public CRhRdkSdkRender
{
public:
	CSampleRdkAsyncRendererSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn& pPlugin,
	                                 const ON_wString& sCaption, UINT idIcon, bool bPreview);

public:
	// CRhRdkSdkRender overrides
	virtual BOOL32 RenderSceneWithNoMeshes(void) override { return true; }
	virtual BOOL32 RenderEnterModalLoop() override { return true; } // Even though it's not modal, we must return true.
	virtual BOOL32 RenderContinueModal()  override { return true; } // Even though it's not modal, we must return true.
	virtual BOOL32 RenderExitModalLoop()  override { return true; } // Even though it's not modal, we must return true.
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
};
