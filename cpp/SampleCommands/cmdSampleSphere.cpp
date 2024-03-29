#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSphere command
//

class CCommandSampleSphere : public CRhinoCommand
{
public:
  CCommandSampleSphere() = default;
  ~CCommandSampleSphere() = default;
  UUID CommandUUID() override
  {
    // {9D098DFD-77D6-4025-802D-73F125EB967C}
    static const GUID SampleSphereCommand_UUID =
    { 0x9D098DFD, 0x77D6, 0x4025, { 0x80, 0x2D, 0x73, 0xF1, 0x25, 0xEB, 0x96, 0x7C } };
    return SampleSphereCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSphere"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleSphere object
static class CCommandSampleSphere theSampleSphereCommand;

CRhinoCommand::result CCommandSampleSphere::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoCommand::result rc = CRhinoCommand::cancel;

  ON_3dPoint center(0.0, 0.0, 0.0);
  double radius = 5.0;
  ON_Sphere sphere(center, radius);
  ON_RevSurface* sphere_srf = sphere.RevSurfaceForm(true);
  if (0 == sphere_srf)
    return rc;

  CRhinoSurfaceObject* sphere_obj = new CRhinoSurfaceObject();
  sphere_obj->SetSurface(sphere_srf);

  if (doc->AddObject(sphere_obj))
  {
    doc->Redraw();
    rc = CRhinoCommand::success;
  }
  else
  {
    delete sphere_obj;
    sphere_obj = 0;
  }

  return rc;
}

//
// END SampleSphere command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
