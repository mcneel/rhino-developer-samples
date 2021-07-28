
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
	return DrawOrLockRendererFrameBufferImpl(*this, *m_Renderer.RenderWindow(), input, outputs);
}

void CSampleDisplayMode::UnlockRendererFrameBuffer()
{
	UnlockRendererFrameBufferImpl(*this, *m_Renderer.RenderWindow());
}

bool CSampleDisplayMode::UseFastDraw()
{
	return false;
}
