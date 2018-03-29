
#include "stdafx.h"
#include "MarmaladeRenderer.h"
#include "MarmaladeSdkRender.h"

static float Random(void)
{
	return float(rand()) / 32767.0f;
}

CMarmaladeRenderer::CMarmaladeRenderer(CMarmaladeSdkRender& sdkRender, const ON_3dmRenderSettings& rs)
	:
	m_RenderSize(0, 0),
	m_pRectRegion(NULL),
	m_bContinueRendering(false),
	m_SdkRender(sdkRender),
	m_RenderSettings(rs),
	m_eventRenderEnd(false, true /* Manual reset */)
{
}

CMarmaladeRenderer::~CMarmaladeRenderer(void)
{
}

void CMarmaladeRenderer::SetRenderRegion(const LPCRECT pRectRegion)
{
	m_pRectRegion = pRectRegion;
}

void CMarmaladeRenderer::SetRenderSize(const CSize& size)
{
	m_RenderSize = size;
}

bool CMarmaladeRenderer::Render(void)
{
	CRhinoDoc* pDoc = RhinoApp().ActiveDoc();
	if (NULL == pDoc)
		return false;

	bool bSuccess = true;

	m_bContinueRendering = true;

	m_eventRenderEnd.ResetEvent();

	const bool bPreview = m_SdkRender.RenderQuick();
	IRhRdkRenderWindow& renderWindow = m_SdkRender.GetRenderWindow();

	CRect region(0, 0, m_RenderSize.cx, m_RenderSize.cy);

	if (NULL != m_pRectRegion)
	{
		region = *m_pRectRegion;
	}

	renderWindow.SetSize(region.Size());

	CRhinoView* pView = RhinoApp().ActiveView();
	if (NULL != pView)
	{
		renderWindow.AddWireframeChannel(*pDoc, pView->Viewport().View(), m_RenderSize, region);
	}

	IRhRdkRenderWindow::IChannel* pChannel = renderWindow.OpenChannel(IRhRdkRenderWindow::chanRGBA);

	CRect rect(0, 0, region.Width(), 0);

	int percent = 0;

	const int width  = region.Width();
	const int height = region.Height();

	int samples = 1, sampleRate = 1;
	if (!bPreview)
	{
		switch (m_RenderSettings.m_antialias_style)
		{
		case 1: sampleRate = 4;  break;
		case 2: sampleRate = 16; break;
		}

		samples = sampleRate * sampleRate;
	}

	const float pi = float(ON_PI);

	const float size = float(min(width, height));

	const CPoint origin(region.left, region.bottom);

	const int mosaic = 8;

	const int delta = bPreview ? mosaic : 1;

	for (int y = region.top; y < region.bottom; y += delta)
	{
		// Allow stopping from another thread.
		if (!m_bContinueRendering)
		{
			bSuccess = false;
			break;
		}

		const int pixelY = y - region.top;

		for (int x = region.left; x < region.right; x += delta)
		{
			const int pixelX = x - region.left;

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
						bool b = false;
						if (t0 < 0.0010f) { weight += (0.001f - t0) * 800.0f; b = true; }
						if (t1 < 0.0010f) { weight += (0.001f - t1) * 800.0f; b = true; }
						if (t2 < 0.0010f) { weight += (0.001f - t2) * 800.0f; b = true; }
						if (t3 < 0.0010f) { weight += (0.001f - t3) * 800.0f; b = true; }

						if (!b && (t6 < 0.0030f)) weight += (0.0300f - t6) * 5.0f;
					}

					r += 0.9f * weight + 0.9f;
					g += 0.5f * weight + 0.3f + 0.5f * fmod(Random(), fabs(dx) + 1e-6f);
					b += 0.2f * weight + 0.1f + 0.5f * fmod(Random(), fabs(dy) + 1e-6f);
					a += weight;
				}
				else
				{
					r += 0.2f; g += 0.5f; b += 0.9f; a += 1.0f;
				}
			}

			float rgba[4];
			rgba[0] = max(0.0f, min(1.0f, r / samples));
			rgba[1] = max(0.0f, min(1.0f, g / samples));
			rgba[2] = max(0.0f, min(1.0f, b / samples));
			rgba[3] = max(0.0f, min(1.0f, a / samples));

			if (bPreview)
			{
				for (int j = 0; j < mosaic; j++)
				{
					for (int i = 0; i < mosaic; i++)
					{
						pChannel->SetValue(pixelX+i, pixelY+j, &rgba);
					}
				}
			}
			else
			{
				pChannel->SetValue(pixelX, pixelY, &rgba);
			}
		}

		rect.top = pixelY;
		rect.bottom = pixelY + 1;
		renderWindow.InvalidateArea(rect);

		percent = (pixelY * 100) / height;
		renderWindow.SetProgress(L"Rendering...", percent);
	}

	if (bSuccess)
	{
		percent = 100;
	}

	pChannel->Close();

	renderWindow.SetProgress(bSuccess ? L"Completed" : L"Aborted", percent);

	m_eventRenderEnd.SetEvent();

	return bSuccess;
}

void CMarmaladeRenderer::StopRendering(void)
{
	// Called in the context of the main thread.
	// This method actually gets called twice when the user cancels the rendering. Once by RDK
	// in order to stop the rendering, and then again by Rhino for reasons unknown. It's important
	// to make sure the second time doesn't wait for rendering to stop again which is the reason that
	// we use a manual event that stays signalled. Also, once this starts waiting for the end event
	// to be signalled, the worker thread must not do anything that could wait on the main thread
	// or a deadlock will result.

	m_bContinueRendering = false;

	::WaitForSingleObject(m_eventRenderEnd, INFINITE);
}
