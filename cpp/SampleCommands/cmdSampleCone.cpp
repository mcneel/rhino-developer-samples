#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCone command
//

class CCommandSampleCone : public CRhinoCommand
{
public:
  CCommandSampleCone() = default;
  ~CCommandSampleCone() = default;
  UUID CommandUUID() override
  {
    // {4857D22E-6110-4FCD-A05C-9E51AE670B27}
    static const GUID SampleConeCommand_UUID =
    { 0x4857D22E, 0x6110, 0x4FCD, { 0xA0, 0x5C, 0x9E, 0x51, 0xAE, 0x67, 0x0B, 0x27 } };
    return SampleConeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCone"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleCone object
static class CCommandSampleCone theSampleConeCommand;

CRhinoCommand::result CCommandSampleCone::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_Plane plane = ON_xy_plane;
  double height = 10.0;
  double radius = 5.0;
  BOOL bCapBottom = FALSE;

  ON_Cone cone(plane, height, radius);
  if (cone.IsValid())
  {
    ON_Brep* cone_brep = ON_BrepCone(cone, bCapBottom);
    if (cone_brep)
    {
      CRhinoBrepObject* cone_object = new CRhinoBrepObject();
      cone_object->SetBrep(cone_brep);
      doc->AddObject(cone_object);
      doc->Redraw();
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleCone command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
