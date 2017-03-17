#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSurfaceArea command
//

#pragma region SampleSurfaceArea command

class CCommandSampleSurfaceArea : public CRhinoCommand
{
public:
  CCommandSampleSurfaceArea() {}
  ~CCommandSampleSurfaceArea() {}
  UUID CommandUUID()
  {
    // {9A978BBA-DDE8-4C87-8A58-8B887442B868}
    static const GUID SampleSurfaceAreaCommand_UUID =
    { 0x9A978BBA, 0xDDE8, 0x4C87, { 0x8A, 0x58, 0x8B, 0x88, 0x74, 0x42, 0xB8, 0x68 } };
    return SampleSurfaceAreaCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSurfaceArea"; }
  const wchar_t* LocalCommandName() const { return L"SampleSurfaceArea"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSurfaceArea object
static class CCommandSampleSurfaceArea theSampleSurfaceAreaCommand;

CRhinoCommand::result CCommandSampleSurfaceArea::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surfaces or polysurfaces area calculation");
  go.SetGeometryFilter(CRhinoObject::surface_object | CRhinoObject::polysrf_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  int i, object_count = go.ObjectCount();

  ON_SimpleArray<const ON_Brep*> breps;
  for (i = 0; i < object_count; i++)
  {
    const ON_Brep* brep = go.Object(i).Brep();
    if (0 == brep)
      return CRhinoCommand::failure;
    breps.Append(brep);
  }

  ON_SimpleArray<ON_MassProperties> areas(object_count);
  for (i = 0; i < object_count; i++)
  {
    ON_MassProperties mp;
    if (breps[i]->AreaMassProperties(mp, true, false, false, false, 1e-6, 1e-6))
      areas.Append(mp);
  }

  ON_MassProperties results;
  results.Sum(areas.Count(), areas.Array());

  RhinoApp().Print(L"Area = %f square units\n", results.Area());

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSurfaceArea command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
