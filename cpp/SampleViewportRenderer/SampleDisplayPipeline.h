#pragma once

// This is our custom display pipeline. It will copy the
// rendered image to the viewport.

class CSampleDisplayPipeline : public CRhinoDisplayPipeline
{
  // This is needed to declare and register the pipeline.
  DECLARE_DYNAMIC_PIPELINE(CSampleDisplayPipeline)

public:
  CSampleDisplayPipeline() = default;
  ~CSampleDisplayPipeline() = default;;

  // Override the CRhinoDisplayPipeline::ShowFrameBuffer.
  // This is called when Rhino wants the pipeline to show
  // it's back buffer.
  virtual bool ShowFrameBuffer(HDC hDC) override;
};