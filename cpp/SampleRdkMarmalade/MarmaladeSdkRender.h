
#pragma once

class CMarmaladeRenderer;

class CMarmaladeSdkRender : public CRhRdkSdkRender
{
public:
	CMarmaladeSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn& plugIn,
	                    const ON_wString& sCaption, UINT uIconId, bool bPreview);

	virtual ~CMarmaladeSdkRender();

	int ThreadedRender(void);
	void SetContinueModal(bool b);
	bool RenderQuick(void) const;

	// CRhRdkSdkRender overrides.
	virtual CRhinoSdkRender::RenderReturnCodes Render(const ON_2iSize& sizeImage) override;
	virtual CRhinoSdkRender::RenderReturnCodes RenderWindow(CRhinoView* pView, const LPRECT pRect, bool bInWindow) override;
	virtual BOOL RenderSceneWithNoMeshes() override { return TRUE; }
	virtual BOOL IgnoreRhinoObject(const CRhinoObject*) override { return FALSE; }
	virtual BOOL RenderPreCreateWindow() override;
	virtual BOOL RenderEnterModalLoop() override{ return TRUE; }
	virtual BOOL RenderContinueModal() override;
	virtual BOOL RenderExitModalLoop() override{ return TRUE; }
	virtual bool ReuseRenderWindow(void) const override { return true; }
	virtual BOOL NeedToProcessGeometryTable() override;
	virtual BOOL NeedToProcessLightTable() override;
	virtual BOOL StartRenderingInWindow(CRhinoView* pView, const LPCRECT pRect) override;
	virtual void StopRendering() override;
	virtual void StartRendering() override;

private:
	bool m_bRenderQuick;
	bool m_bContinueModal = true;
	CMarmaladeRenderer* m_pRenderer = nullptr;
};
