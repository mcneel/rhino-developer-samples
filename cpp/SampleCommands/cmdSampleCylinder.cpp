#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCylinder command
//

class CCommandSampleCylinder : public CRhinoCommand
{
public:
  CCommandSampleCylinder() = default;
  ~CCommandSampleCylinder() = default;
  UUID CommandUUID() override
  {
    // {D5024CAB-8C2A-4F05-BC21-8FD5582C31B1}
    static const GUID SampleCylinderCommand_UUID =
    { 0xD5024CAB, 0x8C2A, 0x4F05, { 0xBC, 0x21, 0x8F, 0xD5, 0x58, 0x2C, 0x31, 0xB1 } };
    return SampleCylinderCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCylinder"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleCylinder object
static class CCommandSampleCylinder theSampleCylinderCommand;

CRhinoCommand::result CCommandSampleCylinder::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_3dPoint center_point(0.0, 0.0, 0.0);
  double radius = 5.0;
  ON_3dPoint height_point(0.0, 0.0, 10.0);
  ON_3dVector zaxis = height_point - center_point;
  ON_Plane plane(center_point, zaxis);
  ON_Circle circle(plane, radius);
  ON_Cylinder cylinder(circle, zaxis.Length());
  ON_Brep* brep = ON_BrepCylinder(cylinder, TRUE, TRUE);
  if (brep)
  {
    CRhinoBrepObject* cylinder_object = new CRhinoBrepObject();
    cylinder_object->SetBrep(brep);
    if (doc->AddObject(cylinder_object))
      doc->Redraw();
    else
      delete cylinder_object;
  }

  return CRhinoCommand::success;
}
//
// END SampleCylinder command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
