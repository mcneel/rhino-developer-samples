#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCaptureFrameBuffer command
//

#pragma region SampleCaptureFrameBuffer command

class CCommandSampleCaptureFrameBuffer : public CRhinoCommand
{
public:
  CCommandSampleCaptureFrameBuffer() {}
  ~CCommandSampleCaptureFrameBuffer() {}
  UUID CommandUUID()
  {
    // {22292D24-C175-4CEE-B299-2BC55665014C}
    static const GUID SampleCaptureFrameBufferCommand_UUID =
    { 0x22292D24, 0xC175, 0x4CEE, { 0xB2, 0x99, 0x2B, 0xC5, 0x56, 0x65, 0x01, 0x4C } };
    return SampleCaptureFrameBufferCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCaptureFrameBuffer"; }
  const wchar_t* LocalCommandName() const { return L"SampleCaptureFrameBuffer"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleCaptureFrameBuffer object
static class CCommandSampleCaptureFrameBuffer theSampleCaptureFrameBufferCommand;

CRhinoCommand::result CCommandSampleCaptureFrameBuffer::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view)
    return CRhinoCommand::failure;

  CRhinoDisplayPipeline* dp = view->DisplayPipeline();
  if (0 == dp)
    return CRhinoCommand::failure;

  if (dp->CaptureFrameBuffer(true))
  {
    const CRhinoUiDib& fb = dp->GetFrameBuffer();
    // Now use ‘fb’ to do whatever you want - it’s basically a
    // DIB containing the current frame buffer for the given pipeline.
  }
 
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCaptureFrameBuffer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
