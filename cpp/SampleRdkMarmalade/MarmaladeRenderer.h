
#pragma once

class CMarmaladeSdkRender;

/** \class MarmaladeRenderer

	This class implements a demo renderer. It just renders a procedural orange slice :-)
*/
class CMarmaladeRenderer
{
public:
	CMarmaladeRenderer(CMarmaladeSdkRender& sdkRender, const ON_3dmRenderSettings& rs);
	~CMarmaladeRenderer(void);

	void SetRenderSize(const CSize& size);
	void SetRenderRegion(const LPCRECT pRectRegion);

	bool Render(void);
	void StopRendering(void);

private:
	LPCRECT m_pRectRegion; // For RenderInWindow and RenderWindow.
	const ON_3dmRenderSettings& m_RenderSettings;
	CMarmaladeSdkRender& m_SdkRender;
	CEvent m_eventRenderEnd;
	CSize m_RenderSize;
	bool m_bContinueRendering;
};
