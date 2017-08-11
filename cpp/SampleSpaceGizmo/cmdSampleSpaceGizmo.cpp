#include "StdAfx.h"
#include "SampleSpaceGizmoPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSpaceGizmo command
//

#pragma region SampleSpaceGizmo command

class CCommandSampleSpaceGizmo : public CRhinoCommand
{
public:
  CCommandSampleSpaceGizmo() = default;
  ~CCommandSampleSpaceGizmo() = default;
  UUID CommandUUID() override
  {
    // {DB2A1D6B-A8F3-41EE-ABC7-3AAA70C81F4A}
    static const GUID SampleSpaceGizmoCommand_UUID =
    { 0xDB2A1D6B, 0xA8F3, 0x41EE, { 0xAB, 0xC7, 0x3A, 0xAA, 0x70, 0xC8, 0x1F, 0x4A } };
    return SampleSpaceGizmoCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSpaceGizmo"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleSpaceGizmo object
static class CCommandSampleSpaceGizmo theSampleSpaceGizmoCommand;

CRhinoCommand::result CCommandSampleSpaceGizmo::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  // This will bring up Rhino's Options dialog with our page selected
  RhinoApp().RunScript(context.m_doc.RuntimeSerialNumber(), L"! _OptionsPage _SpaceGizmo", 0);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSpaceGizmo command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
