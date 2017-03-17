#include "StdAfx.h"
#include "SampleViewportRendererPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleViewportRenderer command
//

#pragma region SampleViewportRenderer command

class CCommandSampleViewportRenderer : public CRhinoCommand
{
public:
  CCommandSampleViewportRenderer() {}
  ~CCommandSampleViewportRenderer() {}
  UUID CommandUUID()
  {
    // {12352A6D-4707-4FB2-A953-7EFA6E255216}
    static const GUID SampleViewportRendererCommand_UUID =
    { 0x12352A6D, 0x4707, 0x4FB2, { 0xA9, 0x53, 0x7E, 0xFA, 0x6E, 0x25, 0x52, 0x16 } };
    return SampleViewportRendererCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleViewportRenderer"; }
  const wchar_t* LocalCommandName() const { return L"SampleViewportRenderer"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleViewportRenderer object
static class CCommandSampleViewportRenderer theSampleViewportRendererCommand;

CRhinoCommand::result CCommandSampleViewportRenderer::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);
  RhinoApp().Print(L"%s plug-in loaded.\n", SampleViewportRendererPlugIn().PlugInName());
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleViewportRenderer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
