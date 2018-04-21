
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

	// CRhRdkSdkRender overrides
	virtual CRhinoSdkRender::RenderReturnCodes Render(const ON_2iSize& sizeImage) override;
	virtual CRhinoSdkRender::RenderReturnCodes RenderWindow(CRhinoView* pView, const LPRECT pRect, bool bInWindow) override;
	virtual BOOL RenderSceneWithNoMeshes()				{ return TRUE; }
	virtual BOOL IgnoreRhinoObject(const CRhinoObject*)	{ return FALSE; }
	virtual BOOL RenderPreCreateWindow();
	virtual BOOL RenderEnterModalLoop()					{ return TRUE; }
	virtual BOOL RenderContinueModal();
	virtual BOOL RenderExitModalLoop()					{ return TRUE; }
	virtual bool ReuseRenderWindow(void) const          { return true; }
	virtual BOOL NeedToProcessGeometryTable();
	virtual BOOL NeedToProcessLightTable();
	virtual BOOL StartRenderingInWindow(CRhinoView* pView, const LPCRECT pRect);
	virtual void StopRendering();
	virtual void StartRendering();

private:
	bool m_bRenderQuick;
	bool m_bContinueModal = true;
	unsigned int m_uRhinoDocSerial = 0;
	CMarmaladeRenderer* m_pRenderer = nullptr;
};
