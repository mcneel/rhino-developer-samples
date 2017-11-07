
#pragma once

#include "SampleRenderer.h"


static const ON_UUID& DisplayModeId(void)
{
	// {2F7325CD-98A8-4628-B96C-75131D02CC18}
	static const GUID id = { 0x2f7325cd, 0x98a8, 0x4628, { 0xb9, 0x6c, 0x75, 0x13, 0x1d, 0x2, 0xcc, 0x18 } };
	return id;
}


class CSampleDisplayMode : public RhRdk::Realtime::DisplayMode
{
public:
	CSampleDisplayMode(const CRhinoDisplayPipeline& pipeline);
	virtual ~CSampleDisplayMode();

public:
	virtual const UUID& ClassId(void) const override
	{
		return DisplayModeId();
	}

	//	The display pipeline has successfully requested a lock on the renderer
	//	and we can use this to render the viewport.
	//	pParent points to the original engine if this rendering is being used for capture (render an existing view at a new size)
	virtual bool StartRenderer(const ON_2iSize& frameSize, const CRhinoDoc& doc, const ON_3dmView& view, const ON_Viewport& vp, const DisplayMode* pParent) override;

	// Scene has changed, don't start from scratch, just get changes and continue
	virtual bool OnRenderSizeChanged(const ON_2iSize& newFrameSize) override;
	virtual void ShutdownRenderer() override;

	//The system's best guess as to whether StartRenderer will succeed.
	virtual bool RendererIsAvailable(void) const override;

	// change queue
	virtual void CreateWorld(const CRhinoDoc& doc, const ON_3dmView& view, const CDisplayPipelineAttributes& attributes) override;

	//Used to pass information about which pass to render to for cloned (view capture) renderers.
	virtual int LastRenderedPass(void) const override;

	//Supports for the built-in view capture progress dialog
	virtual bool ShowCaptureProgress(void) const override;
	virtual double Progress(void) const override;

	//	We have started the renderer and set it up successfully.
	virtual bool IsRendererStarted(void) const override;
			
	// A renderer has completed rendering requested passes
	virtual bool IsCompleted(void) const override;

	// Called during the frame draw to decide whether there is relevant data
	// when "LockRendererFrameBuffer" is called.  Return false if the image is still
	// being rendered.
	virtual bool IsFrameBufferAvailable(const ON_3dmView& vp) const override;

	// Return true if you have drawn the frame into the current OpenGL view, or you have supplied one of the other
	// methods for drawing in outputs.  Return false if the frame was not available, and the framework should wait.
	virtual bool DrawOrLockRendererFrameBuffer(const FRAME_BUFFER_INFO_INPUTS& input, FRAME_BUFFER_INFO_OUTPUTS& outputs) override;
	virtual void UnlockRendererFrameBuffer() override;

	// Return true for fast draw mechanism. Default is old behavior where the pipeline would wait for
	// a short period on several occasions for the implementor to get a framebuffer ready.
	// This function is called just before StartRenderer();
	virtual bool UseFastDraw() override;

private:
	CSampleRenderer m_Renderer;
};



class CDisplayModeFactory : public RhRdk::Realtime::DisplayMode::Factory
{
public:
	virtual ON_wString Name(void) const override
	{
		return L"SampleRealtimeRenderer";
	}

	virtual const UUID& ClassId(void) const override
	{
		return DisplayModeId();
	}

	virtual std::shared_ptr<RhRdk::Realtime::DisplayMode> MakeDisplayEngine(const CRhinoDisplayPipeline& pipeline) const override
	{
		return std::make_shared<CSampleDisplayMode>(pipeline);
	}

	void * EVF(const wchar_t*, void*) override { return nullptr; }
};
