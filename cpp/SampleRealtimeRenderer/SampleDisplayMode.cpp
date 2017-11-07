
#include "stdafx.h"
#include "SampleDisplayMode.h"


CSampleDisplayMode::CSampleDisplayMode(const CRhinoDisplayPipeline& pipeline)
: RhRdk::Realtime::DisplayMode(pipeline),
  m_Renderer(this)
{
}

CSampleDisplayMode::~CSampleDisplayMode()
{
	if (m_Renderer.Running())
	{
		m_Renderer.StopRenderProcess();
	}
}

bool CSampleDisplayMode::StartRenderer(const ON_2iSize& frameSize, const CRhinoDoc& doc, const ON_3dmView& view, const ON_Viewport& vp, const DisplayMode* pParent)
{
	if (m_Renderer.Running())
	{
		m_Renderer.StopRenderProcess();
	}

	return m_Renderer.StartRenderProcess(frameSize);
}

bool CSampleDisplayMode::OnRenderSizeChanged(const ON_2iSize& newFrameSize)
{
	if (m_Renderer.Running())
	{
		m_Renderer.StopRenderProcess();
	}

	return m_Renderer.StartRenderProcess(newFrameSize);
}

void CSampleDisplayMode::ShutdownRenderer()
{
	if (m_Renderer.Running())
	{
		m_Renderer.StopRenderProcess();
	}
}

bool CSampleDisplayMode::RendererIsAvailable(void) const
{
	return true;
}

void CSampleDisplayMode::CreateWorld(const CRhinoDoc& doc, const ON_3dmView& view, const CDisplayPipelineAttributes& attributes)
{

}

int CSampleDisplayMode::LastRenderedPass(void) const
{
	return 0;
}

bool CSampleDisplayMode::ShowCaptureProgress(void) const
{
	return false;
}

double CSampleDisplayMode::Progress(void) const
{
	return 0.0;
}

bool CSampleDisplayMode::IsRendererStarted(void) const
{
	return m_Renderer.Running();
}

bool CSampleDisplayMode::IsCompleted(void) const
{
	return false;
}

bool CSampleDisplayMode::IsFrameBufferAvailable(const ON_3dmView& vp) const
{
	return true;
}

bool CSampleDisplayMode::DrawOrLockRendererFrameBuffer(const FRAME_BUFFER_INFO_INPUTS& input, FRAME_BUFFER_INFO_OUTPUTS& outputs)
{
	if(!outputs.client_render_success)
	{
		if (nullptr == m_Renderer.RenderWindow())
			return false;

		const CRhinoDib* pDib = m_Renderer.RenderWindow()->LockDib();
		if (nullptr == pDib)
			return false;

		outputs.pointer_to_dib = pDib;
	}

	return true;
}

void CSampleDisplayMode::UnlockRendererFrameBuffer()
{
	if (nullptr != m_Renderer.RenderWindow())
		m_Renderer.RenderWindow()->UnlockDib();
}

bool CSampleDisplayMode::UseFastDraw()
{
	return false;
}
