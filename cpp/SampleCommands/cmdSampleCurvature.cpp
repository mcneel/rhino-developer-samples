#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCurvature command
//

class CCommandSampleCurvature : public CRhinoCommand
{
public:
  CCommandSampleCurvature() {}
  ~CCommandSampleCurvature() {}
  UUID CommandUUID()
  {
    // {DBB19D32-AF24-40A1-BE43-A14D7E08413A}
    static const GUID SampleCurvatureCommand_UUID =
    { 0xDBB19D32, 0xAF24, 0x40A1, { 0xBE, 0x43, 0xA1, 0x4D, 0x7E, 0x08, 0x41, 0x3A } };
    return SampleCurvatureCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCurvature"; }
  const wchar_t* LocalCommandName() { return L"SampleCurvature"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCurvature object
static class CCommandSampleCurvature theSampleCurvatureCommand;

CRhinoCommand::result CCommandSampleCurvature::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve for curvature measurement");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* crv = go.Object(0).Curve();
  if (0 == crv)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Select point on curve for curvature measurement");
  gp.Constrain(*crv);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint pt = gp.Point();

  double t = 0.0;
  if (!crv->GetClosestPoint(pt, &t))
  {
    RhinoApp().Print(L"Failed to compute radius of curvature.\n");
    return CRhinoCommand::failure;
  }

  ON_3dVector tangent = crv->TangentAt(t);
  if (tangent.IsTiny())
  {
    RhinoApp().Print(L"Failed to compute radius of curvature. Curve may have stacked control points.\n");
    return CRhinoCommand::failure;
  }

  ON_3dVector curvature = crv->CurvatureAt(t);
  const double k = curvature.Length();
  if (k < ON_SQRT_EPSILON)
  {
    RhinoApp().Print(L"Radius of curvature: infinite.\n");
    return CRhinoCommand::failure;
  }

  ON_3dVector radius_vector = curvature / (k * k);
  ON_Circle circle;
  if (!circle.Create(pt, tangent, pt + 2.0 * radius_vector))
  {
    RhinoApp().Print(L"Failed to compute radius of curvature.\n");
    return CRhinoCommand::failure;
  }

  context.m_doc.AddCurveObject(circle);
  context.m_doc.AddPointObject(pt);
  context.m_doc.Redraw();

  ON_wString wRadius;
  RhinoFormatNumber(circle.Radius(), wRadius);
  RhinoApp().Print(L"Radius of curvature: %s.\n", wRadius);

  return CRhinoCommand::success;
}
//
// END SampleCurvature command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
