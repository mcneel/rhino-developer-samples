#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomRenderMesh command
//

#pragma region SampleCustomRenderMesh command

class CCommandSampleCustomRenderMesh : public CRhinoCommand
{
public:
  CCommandSampleCustomRenderMesh() = default;
  ~CCommandSampleCustomRenderMesh() = default;
  UUID CommandUUID() override
  {
    // {6BBAFB33-6E7B-43F7-8CE9-DE4CAADFB5BE}
    static const GUID SampleCustomRenderMeshCommand_UUID =
    { 0x6BBAFB33, 0x6E7B, 0x43F7, { 0x8C, 0xE9, 0xDE, 0x4C, 0xAA, 0xDF, 0xB5, 0xBE } };
    return SampleCustomRenderMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCustomRenderMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleCustomRenderMesh object
static class CCommandSampleCustomRenderMesh theSampleCustomRenderMeshCommand;

CRhinoCommand::result CCommandSampleCustomRenderMesh::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_MeshParameters mp = doc->Properties().MeshParameters(ON_MeshParameters::MESH_STYLE::render_mesh_custom);
  double relative_tolerance = mp.RelativeTolerance();

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Custom render mesh density");
  gn.SetDefaultNumber(relative_tolerance);
  gn.SetLowerLimit(0.0, false);
  gn.SetUpperLimit(1.0, false);
  gn.AcceptNothing();
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  double new_relative_tolerance = gn.Number();
  if (new_relative_tolerance == relative_tolerance)
    return CRhinoCommand::nothing;

  if (ON_IsValid(new_relative_tolerance))
  {
    new_relative_tolerance = RHINO_CLAMP(new_relative_tolerance, 0.0, 1.0);
    mp.SetRelativeTolerance(new_relative_tolerance);
    mp.SetRefine(new_relative_tolerance < 0.65);
    mp.SetSimplePlanes(0.0 == new_relative_tolerance);
  }

  doc->Properties().SetCustomRenderMeshParameters(mp);
  doc->Properties().SetRenderMeshStyle(ON_MeshParameters::MESH_STYLE::render_mesh_custom);
  doc->Regen();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomRenderMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
