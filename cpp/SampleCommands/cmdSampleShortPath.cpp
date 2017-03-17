#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleShortPath command
//

#pragma region SampleShortPath command

class CCommandSampleShortPath : public CRhinoCommand
{
public:
  CCommandSampleShortPath() {}
  ~CCommandSampleShortPath() {}
  UUID CommandUUID()
  {
    // {12FF87FF-B55F-4492-B455-412B861FFEC4}
    static const GUID SampleShortPathCommand_UUID =
    { 0x12FF87FF, 0xB55F, 0x4492, { 0xB4, 0x55, 0x41, 0x2B, 0x86, 0x1F, 0xFE, 0xC4 } };
    return SampleShortPathCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleShortPath"; }
  const wchar_t* LocalCommandName() const { return L"SampleShortPath"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleShortPath object
static class CCommandSampleShortPath theSampleShortPathCommand;

CRhinoCommand::result CCommandSampleShortPath::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface for creating a short curve between two points on the surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Surface* srf = go.Object(0).Surface();
  if (0 == srf)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Start of curve");
  gp.Constrain(*srf);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  double u0, v0;
  if (!gp.PointOnSurface(&u0, &v0))
  {
    RhinoApp().Print(L"Point not on the surface.\n");
    return CRhinoCommand::failure;
  }

  gp.SetCommandPrompt(L"End of curve");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  double u1, v1;
  if (!gp.PointOnSurface(&u1, &v1))
  {
    RhinoApp().Print(L"Point not on the surface.\n");
    return CRhinoCommand::failure;
  }

  ON_3dPoint p0 = srf->PointAt(u0, v0);
  ON_3dPoint p1 = srf->PointAt(u1, v1);

  if (p0.DistanceTo(p1) < ON_SQRT_EPSILON)
  {
    RhinoApp().Print(L"Points must be distinct.\n");
    return CRhinoCommand::nothing;
  }

  double tol = context.m_doc.AbsoluteTolerance();
  ON_Curve* crv = RhinoShortPath(*srf, ON_2dPoint(u0, v0), ON_2dPoint(u1, v1), tol);
  if (0 == crv)
  {
    RhinoApp().Print(L"Failed to compute shortest path.\n");
    return CRhinoCommand::nothing;
  }

  CRhinoCurveObject* crv_obj = new CRhinoCurveObject();
  crv_obj->SetCurve(crv);
  context.m_doc.AddObject(crv_obj);

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleShortPath command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
