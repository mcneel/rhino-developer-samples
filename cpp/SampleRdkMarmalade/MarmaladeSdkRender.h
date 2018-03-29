
#pragma once

class CMarmaladeRenderer;

class CMarmaladeSdkRender : public CRhRdkSdkRender
{
public:
	CMarmaladeSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn* pPlugin,
	                    const ON_wString& sCaption, UINT idIcon, bool pPreview);

	virtual ~CMarmaladeSdkRender();

	int ThreadedRender(void);
	void SetContinueModal(bool b);
	bool RenderQuick(void) const;

	CRhinoSdkRender::RenderReturnCodes Render(const CSize& sizeRender = RenderSize());
	CRhinoSdkRender::RenderReturnCodes RenderWindow(CRhinoView* pView, const LPRECT pRect, bool bInPopupWindow);

	// CRhRdkSdkRender overrides
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
	bool m_bContinueModal;
	bool m_bRenderQuick;
	CMarmaladeRenderer* m_pRenderer;
};
