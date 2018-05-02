
#include "stdafx.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeSdkRender.h"
#include "MarmaladeRenderer.h"
#include "MarmaladeShader.h"

CMarmaladeSdkRender::CMarmaladeSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn& plugIn,
                                         const ON_wString& sCaption, UINT uIconId, bool bPreview)
	:
	m_bRenderQuick(bPreview),
	CRhRdkSdkRender(context, plugIn, sCaption, uIconId)
{
	const auto* pDoc = context.Document();
	if (nullptr == pDoc)
		return;

	const auto& rs = pDoc->Properties().RenderSettings();
	m_pRenderer = new CMarmaladeRenderer(*this, rs);

	GetRenderWindow().ClearChannels();

	// You can add extra channels here before the rendering starts. This adds a Z-buffer
	// channel which allows things like Fog and DOF post effects to work.
	GetRenderWindow().AddChannel(IRhRdkRenderWindow::chanDistanceFromCamera, sizeof(float));
}

CMarmaladeSdkRender::~CMarmaladeSdkRender()
{
	delete m_pRenderer;
	m_pRenderer = nullptr;
}

bool CMarmaladeSdkRender::RenderQuick(void) const
{
	return m_bRenderQuick;
}

CRhinoSdkRender::RenderReturnCodes CMarmaladeSdkRender::Render(const ON_2iSize& sizeRender)
{
	m_pRenderer->SetRenderSize(sizeRender);

	// NB. Marmalade doesn't use these render meshes, but your plug-in will
	// almost certainly need to access them. We provide this code as an example.

	const ON_Viewport& vp = RhinoApp().ActiveView()->ActiveViewport().VP();

	IRhRdkSdkRenderMeshIterator* pIterator = NewRenderMeshIterator(vp);
	pIterator->EnsureRenderMeshesCreated();

	// You can now use this Iterator to get all of the meshes in the scene.
	// While the iterator is around, all meshes are guaranteed to be available
	// which means you don't need to copy them during the rendering process.
	CRhRdkRenderMesh rm;

	pIterator->Reset();
	while (pIterator->Next(rm))
	{
		CMarmaladeShader* pShader = static_cast<CMarmaladeShader*>(
		                            ::RhRdkObjectMaterialShader(rm.Object(), MarmaladePlugIn().PlugInID()));
		if (nullptr != pShader)
		{
			RhinoApp().Print(pShader->FriendlyName());
			RhinoApp().Print(L"\n");

			// Note - you know that Marmalade materials return a "new CMarmalade????Shader", so that means you
			// are responsible for deleting it.
			delete pShader;
		}
	}

	ON_2iSize s;
	s.cx = sizeRender.cx;
	s.cy = sizeRender.cy;
	const CRhinoSdkRender::RenderReturnCodes rc = CRhRdkSdkRender::Render(s);

	delete pIterator;

	return rc;
}

CRhinoSdkRender::RenderReturnCodes CMarmaladeSdkRender::RenderWindow(CRhinoView* pView, const LPRECT pRect, bool bInPopupWindow)
{
	const auto* pDoc = CommandContext().Document();
	if (nullptr == pDoc)
		return CRhinoSdkRender::RenderReturnCodes::render_empty_scene;

	m_pRenderer->SetRenderRegion(pRect); // Must be before SetRenderSize().

	const auto sizeRender = RenderSize(*pDoc, true);
	m_pRenderer->SetRenderSize(sizeRender);

	// Marmalade doesn't use these render meshes, but your plug-in will
	// almost certainly need to access them. We provide this code as an example.

	const ON_Viewport& vp = pView->ActiveViewport().VP();

	IRhRdkSdkRenderMeshIterator* pIterator = NewRenderMeshIterator(vp);
	pIterator->EnsureRenderMeshesCreated();

	// You can now use this Iterator to get all of the meshes in the scene.
	// While the iterator is around, all meshes are guaranteed to be available
	// which means you don't need to copy them during the rendering process.

	CRhinoSdkRender::RenderReturnCodes rc;

	if (bInPopupWindow)
	{
		// Rendering the specified region in a normal popup window.

		// This method gives roughly a region-sized frame.
		ON_2iSize s;
		const CRect rect = pRect;
		s.cx = rect.Width();
		s.cy = rect.Height();
		rc = CRhRdkSdkRender::Render(s);

		// This method gives a normal-sized frame with a region-sized rendered area inside it.
//		rc = CRhRdkSdkRender::Render(sizeRender);
	}
	else
	{
		// Rendering directly into the viewport.
		rc = CRhRdkSdkRender::RenderWindow(pView, pRect, bInPopupWindow);
	}

	delete pIterator;

	return rc;
}

BOOL CMarmaladeSdkRender::NeedToProcessGeometryTable()
{
	return MarmaladePlugIn().SceneChanged();
}

BOOL CMarmaladeSdkRender::NeedToProcessLightTable()
{
	return MarmaladePlugIn().LightingChanged();
}

BOOL CMarmaladeSdkRender::RenderPreCreateWindow()
{
	MarmaladePlugIn().SetSceneChanged(false);
	MarmaladePlugIn().SetLightingChanged(false);

	return TRUE;
}

BOOL CMarmaladeSdkRender::RenderContinueModal()
{
	return m_bContinueModal;
}

void CMarmaladeSdkRender::SetContinueModal(bool b)
{
	m_bContinueModal = b;
}

void CMarmaladeSdkRender::StopRendering()
{
	m_pRenderer->StopRendering();
}

int CMarmaladeSdkRender::ThreadedRender(void)
{
	m_pRenderer->Render();

	// This must be the last thing the thread does.
	SetContinueModal(false);

	return 0;
}

UINT AFX_CDECL RenderProc(void* pv)
{
	return reinterpret_cast<CMarmaladeSdkRender*>(pv)->ThreadedRender();
}

void CMarmaladeSdkRender::StartRendering()
{
	AfxBeginThread(RenderProc, this);
}

BOOL CMarmaladeSdkRender::StartRenderingInWindow(CRhinoView*, const LPCRECT)
{
	// View and rect are not needed here because they have already been stored/used.

	AfxBeginThread(RenderProc, this);

	return TRUE;
}
