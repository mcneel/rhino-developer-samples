
#include "stdafx.h"
#include "SampleRdkAsyncRendererSdkRender.h"
#include "SampleRdkAsyncRendererPlugIn.h"

class CAsyncRenderContext : public IRhRdkAsyncRenderContext
{
public:
	CAsyncRenderContext(const ON_3dmRenderSettings& rs, bool bPreview);
	virtual ~CAsyncRenderContext();

	int ThreadedRender(void);
	bool RenderCore(void);

public: // Implement IRhRdkAsyncRenderContext.
	virtual void DeleteThis(void) override { delete this; }
	virtual void StopRendering(void) override;
	virtual bool SupportsPause(void) const override { return true; }
	virtual void PauseRendering(void) override;
	virtual void ResumeRendering(void) override;
	virtual void OnQuietRenderFinished(const IRhRdkRenderSession& session) override;
	virtual void* EVF(const wchar_t*, void*) override { return nullptr; }

public:
	CRect m_RectRender;
	CRect m_Region;
	HANDLE m_hRenderThread = NULL;
	IRhRdkRenderWindow* m_pRenderWindow = nullptr;
	ON_3dmRenderSettings m_RenderSettings;
	IRhRdkRenderWindow::IChannel* m_pChanRGBA = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanZ = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalX = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalY = nullptr;
	IRhRdkRenderWindow::IChannel* m_pChanNormalZ = nullptr;
	bool m_bPreview = false;
	bool m_bPause = false;
	volatile bool m_bCancel = false;
};

// CAsyncRenderContext

CAsyncRenderContext::CAsyncRenderContext(const ON_3dmRenderSettings& rs, bool bPreview)
	:
	m_bPreview(bPreview),
	m_RenderSettings(rs)
{
}

CAsyncRenderContext::~CAsyncRenderContext()
{
	// It is important to ensure that the render thread is gone and the thread handle is closed.
	StopRendering();
}

void CAsyncRenderContext::StopRendering(void)
{
	// If rendering is in progress, cancel it and wait for it to stop.
	if (NULL != m_hRenderThread)
	{
		m_bCancel = true;

		::WaitForSingleObject(m_hRenderThread, INFINITE);
		::CloseHandle(m_hRenderThread);
		m_hRenderThread = NULL;
	}
}

void CSampleRdkAsyncRendererSdkRender::StopRendering()
{
	// This is only called for synchronous renderers.
}

void CAsyncRenderContext::PauseRendering(void)
{
	m_bPause = true;
}

void CAsyncRenderContext::ResumeRendering(void)
{
	m_bPause = false;
}

static float Random(void)
{
	return float(rand()) / RAND_MAX;
}

bool CAsyncRenderContext::RenderCore(void)
{
	// TODO: Replace this with your own rendering code.

	if ((nullptr == m_pChanRGBA) || (nullptr == m_pChanZ))
		return false;

	ON_4iRect rect(0, 0, m_Region.Width(), 0);

	int percent = 0;

	const int width  = m_Region.Width();
	const int height = m_Region.Height();

	int samples = 1, sampleRate = 1;
	if (!m_bPreview)
	{
		switch (m_RenderSettings.m_antialias_style)
		{
		case 1: sampleRate = 2;  break;
		case 2: sampleRate = 4;  break;
		case 3: sampleRate = 16; break;
		}

		samples = sampleRate * sampleRate;
	}

	const float pi = float(ON_PI);

	const float size = float(min(width, height));

	const CPoint origin(m_Region.left, m_Region.bottom);

	const int mosaic = 8;

	const int delta = m_bPreview ? mosaic : 1;

	for (int y = m_Region.top; y < m_Region.bottom; y += delta)
	{
		if (m_bCancel)
			break;

		if (0 != (::GetAsyncKeyState(VK_SHIFT) & 0x8000))
			return false; // Press SHIFT to fail (for testing).

		const int pixelY = y - m_Region.top;

		for (int x = m_Region.left; x < m_Region.right; x += delta)
		{
			const int pixelX = x - m_Region.left;

			float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

			for (int i = 0; i < samples; i++)
			{
				const float pixoffx = (i % sampleRate) / float(sampleRate);
				const float pixoffy = (i / sampleRate) / float(sampleRate);

				const float dx = (x - origin.x + pixoffx) / size;
				const float dy = (y - origin.y + pixoffy) / size;

				const float dist2 = (dx * dx) + (dy * dy);
				if (dist2 < 0.95f)
				{
					const float at = fabs(atan2(dy, dx));

					float weight = 0.0f;

					float t0 = at - pi * 0.0f / 6.0f; t0 = t0 * t0;
					float t1 = at - pi * 1.0f / 6.0f; t1 = t1 * t1;
					float t2 = at - pi * 2.0f / 6.0f; t2 = t2 * t2;
					float t3 = at - pi * 3.0f / 6.0f; t3 = t3 * t3;

					float t4 = dist2 - 0.005f; t4 = t4 * t4;
					float t5 = dist2 - 0.950f; t5 = t5 * t5;

					float t6 = fmod(at, pi / 50.0f); t6 = t6 * t6;

					if (t5 < 0.0300f) weight += (0.0300f - t5) / 0.0300f;
					else
					if (t4 < 0.0001f) weight += (0.0001f - t4) / 0.0001f;
					else
					{
						bool bWeight = false;
						if (t0 < 0.0010f) { weight += (0.001f - t0) * 800.0f; bWeight = true; }
						if (t1 < 0.0010f) { weight += (0.001f - t1) * 800.0f; bWeight = true; }
						if (t2 < 0.0010f) { weight += (0.001f - t2) * 800.0f; bWeight = true; }
						if (t3 < 0.0010f) { weight += (0.001f - t3) * 800.0f; bWeight = true; }

						if (!bWeight && (t6 < 0.0030f)) weight += (0.0300f - t6) * 5.0f;
					}

					r += 0.9f * weight + 0.9f;
					g += 0.5f * weight + 0.3f + 0.5f * fmod(Random(), fabs(dx) + 1e-6f);
					b += 0.2f * weight + 0.1f + 0.5f * fmod(Random(), fabs(dy) + 1e-6f);
					a += 1.0f;
				}
				else
				{
					r += 0.2f; g += 0.5f; b += 0.9f;
				}
			}

			float rgba[4];
			rgba[0] = max(0.0f, min(1.0f, r / samples));
			rgba[1] = max(0.0f, min(1.0f, g / samples));
			rgba[2] = max(0.0f, min(1.0f, b / samples));
			rgba[3] = max(0.0f, min(1.0f, a / samples));

			const float z = (rgba[0] + rgba[1] + rgba[2]) / 3.0f;

			const float nx = rgba[0] * 2.0f;
			const float ny = rgba[1] * 2.0f;
			const float nz = rgba[2] * 2.0f;

			if (m_bPreview)
			{
				for (int j = 0; j < mosaic; j++)
				{
					for (int i = 0; i < mosaic; i++)
					{
						const int px = pixelX + i, py = pixelY + j;
						m_pChanRGBA->SetValue(px, py, ComponentOrder::RGBA, &rgba);

						m_pChanZ->SetValue(px, py, ComponentOrder::Irrelevant, &z);

						if (nullptr != m_pChanNormalX)
							m_pChanNormalX->SetValue(px, py, ComponentOrder::Irrelevant, &nx);

						if (nullptr != m_pChanNormalY)
							m_pChanNormalY->SetValue(px, py, ComponentOrder::Irrelevant, &ny);

						if (nullptr != m_pChanNormalZ)
							m_pChanNormalZ->SetValue(px, py, ComponentOrder::Irrelevant, &nz);
					}
				}
			}
			else
			{
				m_pChanRGBA->SetValue(pixelX, pixelY, ComponentOrder::RGBA, &rgba);

				m_pChanZ->SetValue(pixelX, pixelY, ComponentOrder::Irrelevant, &z);

				if (nullptr != m_pChanNormalX)
					m_pChanNormalX->SetValue(pixelX, pixelY, ComponentOrder::Irrelevant, &nx);

				if (nullptr != m_pChanNormalY)
					m_pChanNormalY->SetValue(pixelX, pixelY, ComponentOrder::Irrelevant, &ny);

				if (nullptr != m_pChanNormalZ)
					m_pChanNormalZ->SetValue(pixelX, pixelY, ComponentOrder::Irrelevant, &nz);
			}
		}

		rect.top = pixelY;
		rect.bottom = pixelY + 1;
		m_pRenderWindow->InvalidateArea(rect);

		percent = (pixelY * 100) / height;
		m_pRenderWindow->SetProgress(L"Rendering...", percent);

		if (0 == (::GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			::Sleep(m_bPreview ? 10 : 100); // Deliberate slowdown for testing.
		}

		while (m_bPause && !m_bCancel)
		{
			::Sleep(500);
		}
	}

	return true;
}

int CAsyncRenderContext::ThreadedRender(void)
{
	// This function executes in the context of the worker thread.
	// TODO: Replace this with your own rendering code.

	if (NULL == m_pRenderWindow)
		return -1; // Error. This should never happen.

	m_bCancel = false;

	bool bSuccess = false;

	srand(int(time(nullptr)));

	m_Region = m_RectRender;

	// Open the RGBA channel. This may be all you need.
	m_pChanRGBA = m_pRenderWindow->OpenChannel(IRhRdkRenderWindow::chanRGBA);

	// Optionally open another channel; in this case the Z buffer or distance from camera.
	m_pChanZ = m_pRenderWindow->OpenChannel(IRhRdkRenderWindow::chanDistanceFromCamera);

	// Optionally open another channel; Normal X.
	m_pChanNormalX = m_pRenderWindow->OpenChannel(IRhRdkRenderWindow::chanNormalX);

	// Optionally open another channel; Normal Y.
	m_pChanNormalY = m_pRenderWindow->OpenChannel(IRhRdkRenderWindow::chanNormalY);

	// Optionally open another channel; Normal Z.
	m_pChanNormalZ = m_pRenderWindow->OpenChannel(IRhRdkRenderWindow::chanNormalZ);

	// Run the render.
	bSuccess = RenderCore();

	if (nullptr != m_pChanNormalZ)
		m_pChanNormalZ->Close();

	if (nullptr != m_pChanNormalY)
		m_pChanNormalY->Close();

	if (nullptr != m_pChanNormalX)
		m_pChanNormalX->Close();

	if (nullptr != m_pChanZ)
		m_pChanZ->Close();

	if (nullptr != m_pChanRGBA)
		m_pChanRGBA->Close();

	// In asynchronous renderers, it is critical that we report success or failure of the rendering
	// when it finishes, which could be a long time after the Render() method returned.
	// The IRhRdkRenderWindow interface does not have a way of doing this because it was created
	// before asynchronous rendering was possible, so we work around this by using SetProgress().
	// Passing exactly 100 to SetProgress() means success and passing exactly -100 means failure.
	// NOTE: Canceled renders are considered successful because a user may cancel after many hours
	// of rendering and may still want to keep the completed portion.

	m_pRenderWindow->SetProgress(bSuccess ? L"Completed" : L"Failed", 100);

	const IRhRdkRenderWindow::RenderResult res = bSuccess ? IRhRdkRenderWindow::RenderResult::Completed
	                                                      : IRhRdkRenderWindow::RenderResult::Failed;
	m_pRenderWindow->EndAsyncRender(res);

	return 0;
}

void CAsyncRenderContext::OnQuietRenderFinished(const IRhRdkRenderSession& session)
{
	IRhRdkRenderSession::Status status = session.GetStatus();

	if (IRhRdkRenderSession::Status::Completed == status)
	{
		const auto pChannel = session.RenderWindow().OpenChannel(IRhRdkRenderWindow::chanRGBA);
		if (nullptr != pChannel)
		{
			const auto p = (BYTE*)pChannel->GetValue(0, 0);
			RhinoApp().Print(L": Pixel test: %u\n", *p);

			pChannel->Close();
		}
	}
	else
	if (IRhRdkRenderSession::Status::Failed == status)
		RhinoApp().Print( L": Rendering Failed\n");
}

CSampleRdkAsyncRendererSdkRender::CSampleRdkAsyncRendererSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn& plugIn,
                                 const ON_wString& sCaption, UINT id, bool bPreview)
	:
	CRhRdkSdkRender(context, plugIn, sCaption, id)
{
	// An async render context is an object that establishes a connection between the RDK and the
	// render frame for this render session. Even after this SdkRender object goes off the stack,
	// the render context persists, owned by the associated render frame.

	const auto pDoc = context.Document();
	if (nullptr == pDoc)
		return;

	// It is critical that the render context is created first, before any calls to GetRenderWindow().
	const auto pARC = new CAsyncRenderContext(pDoc->Properties().RenderSettings(), bPreview);
	SetAsyncRenderContext(pARC); // Gives ownership to RDK.

	// Note: GetRenderWindow() uses the render context set above.
	GetRenderWindow().ClearChannels();

	// TODO: Add any extra channels before the rendering starts.

	// e.g., This adds a Z_buffer channel which allows things like Fog and DOF post effects to work.
	GetRenderWindow().AddChannel(IRhRdkRenderWindow::chanDistanceFromCamera, sizeof(float));

	// Add channels for normals.
	GetRenderWindow().AddChannel(IRhRdkRenderWindow::chanNormalX, sizeof(float));
	GetRenderWindow().AddChannel(IRhRdkRenderWindow::chanNormalY, sizeof(float));
	GetRenderWindow().AddChannel(IRhRdkRenderWindow::chanNormalZ, sizeof(float));
}

CAsyncRenderContext* CSampleRdkAsyncRendererSdkRender::AsyncRC(void) const
{
	return dynamic_cast<CAsyncRenderContext*>(AsyncRenderContext());
}

bool CSampleRdkAsyncRendererSdkRender::SetUpRender(CRhinoView* pView, bool bQuiet)
{
	if (nullptr == pView)
		return false;

	if (!::RhRdkIsAvailable())
		return false;

	const ON_Viewport& vp = pView->ActiveViewport().VP();

	// Force render meshes to be created on the main thread.
	IRhRdkSdkRenderMeshIterator* pIterator = NewRenderMeshIterator(vp, true, bQuiet);
	pIterator->EnsureRenderMeshesCreated();

	// You can now use this Iterator to get all of the meshes in the scene.
	// While the iterator is around, all meshes are guaranteed to be available
	// which means you don't need to copy them during the rendering process.

	CRhRdkRenderMesh rm;
	pIterator->Reset();
	while (pIterator->Next(rm))
	{
		// TODO: Use the mesh.  This might be the point you create your acceleration structure
		// or, if you are writing a renderer that uses its own mesh representation, you might do the
		// conversion here.
		// One thing to remember - the IRhRdkSdkRenderMeshIterator::Next function is not, at this time,
		// thread safe, so please don't pass the iterator's pointer into multiple render threads and use
		// it to query the mesh list.  In any case, it's not really optimized for in-render access.
	}

	delete pIterator;

	return true;
}

CRhinoSdkRender::RenderReturnCodes CSampleRdkAsyncRendererSdkRender::Render(const ON_2iSize& sizeImage)
{
	if (!SetUpRender(RhinoApp().ActiveView(), false))
		return CRhinoSdkRender::render_error_starting_render;

	CRhinoSdkRender::RenderReturnCodes rc = __super::Render(sizeImage);

	return rc;
}

CRhinoSdkRender::RenderReturnCodes CSampleRdkAsyncRendererSdkRender::RenderWindow(CRhinoView* pView, const LPRECT pRect, bool bInPopupWindow)
{
	if (!SetUpRender(pView, false))
		return CRhinoSdkRender::render_error_starting_render;

	CRhinoSdkRender::RenderReturnCodes rc;

	if (bInPopupWindow)
	{
		// Rendering the specified region in a normal popup window.

		// This method gives roughly a region-sized frame.
		const CRect rect(pRect);
		const auto cs = rect.Size();
		const auto size = ON_2iSize(cs.cx, cs.cy);

		rc = __super::Render(size);

		// This method gives a normal-sized frame with a region-sized rendered area inside it.
//		rc = __super::Render(sizeRender);
	}
	else
	{
		// Rendering directly into the viewport.
		rc = __super::RenderWindow(pView, pRect, bInPopupWindow);
	}

	return rc;
}

BOOL CSampleRdkAsyncRendererSdkRender::NeedToProcessGeometryTable()
{
	return ::SampleRdkAsyncRendererPlugIn().SceneChanged();
}

BOOL CSampleRdkAsyncRendererSdkRender::NeedToProcessLightTable()
{
	return ::SampleRdkAsyncRendererPlugIn().LightingChanged();
}

BOOL CSampleRdkAsyncRendererSdkRender::RenderPreCreateWindow()
{
	::SampleRdkAsyncRendererPlugIn().SetSceneChanged(FALSE);
	::SampleRdkAsyncRendererPlugIn().SetLightingChanged(FALSE);

	return TRUE;
}

bool CSampleRdkAsyncRendererSdkRender::ReuseRenderWindow(void) const
{
	// This will probably be a user option.
	return true;
}

static int RenderThread(void* pv)
{
	return reinterpret_cast<CAsyncRenderContext*>(pv)->ThreadedRender();
}

void CSampleRdkAsyncRendererSdkRender::StartRendering()
{
	const auto pRC = AsyncRC();
	if (nullptr == pRC)
		return;

	const CRhinoDoc* pDoc = CommandContext().Document();
	if (nullptr == pDoc)
		return;

	const auto size = RenderSize(*pDoc, true);
	pRC->m_RectRender = CRect(0, 0, size.cx, size.cy);

	pRC->m_pRenderWindow = &GetRenderWindow();
	pRC->m_pRenderWindow->SetProgress(L"", 0);

	pRC->m_hRenderThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RenderThread, pRC, 0, NULL);
}

BOOL CSampleRdkAsyncRendererSdkRender::StartRenderingInWindow(CRhinoView*, const LPCRECT rect)
{
	const auto pRC = AsyncRC();
	if (nullptr == pRC)
		return false;

	const CRhinoDoc* pDoc = CommandContext().Document();
	if (nullptr == pDoc)
		return false;

	pRC->m_RectRender = rect;

	pRC->m_pRenderWindow = &GetRenderWindow();
	pRC->m_pRenderWindow->SetProgress(L"", 0);

	pRC->m_hRenderThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RenderThread, pRC, 0, NULL);

	return true;
}
