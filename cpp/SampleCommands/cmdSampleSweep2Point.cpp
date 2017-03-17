#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSweep2Point command
//

// Helper function to calculate rail parameters
static bool GetShapeParameterOnRail(const ON_Curve& shape, const ON_Curve& rail, double tol, double& t
)
{
  ON_Interval rail_domain = rail.Domain();

  // Test start point
  bool rc = rail.GetClosestPoint(shape.PointAtStart(), &t, tol);
  if (rc)
  {
    if (rail.IsClosed())
    {
      if (fabs(t - rail_domain.Max()) < ON_SQRT_EPSILON)
        t = rail_domain.Min();
      if (fabs(t - rail_domain.Min()) < ON_SQRT_EPSILON)
        t = rail_domain.Min();
    }
    return true;
  }

  // Test end point
  rc = rail.GetClosestPoint(shape.PointAtEnd(), &t, tol);
  if (rc)
  {
    if (rail.IsClosed())
    {
      if (fabs(t - rail_domain.Max()) < ON_SQRT_EPSILON)
        t = rail_domain.Min();
      if (fabs(t - rail_domain.Min()) < ON_SQRT_EPSILON)
        t = rail_domain.Min();
    }
    return true;
  }

  // Try intersecting...
  ON_SimpleArray<ON_X_EVENT> x;
  if (1 == rail.IntersectCurve(&shape, x, tol, 0.0) && x[0].IsPointEvent())
  {
    t = x[0].m_a[0];
    if (rail.IsClosed())
    {
      if (fabs(t - rail_domain.Max()) < ON_SQRT_EPSILON)
        t = rail_domain.Min();
    }
    return true;
  }

  return false;
}

// Helper function to create 2 rail sweep
static bool RhinoSweep2Helper(
  CRhinoDoc& doc,
  const ON_Curve* rails[2],
  const ON_SimpleArray<const ON_Curve*>& curves,
  ON_SimpleArray<ON_Brep*>& out_breps,
  const ON_3dPoint* start_pt = 0,
  const ON_3dPoint* end_pt = 0,
  bool bClosed = false,
  bool bSimpleSweep = false,
  bool bSameHeight = false,
  int simplify = 0,
  int rebuild_count = -1
)
{
  int i = 0;

  if (0 == rails[0] || 0 == rails[1] || 0 == curves.Count())
    return false;

  for (i = 0; i < curves.Count(); i++)
  {
    if (0 == curves[i])
      return false;
  }

  double tol = doc.AbsoluteTolerance();
  double angtol = doc.AngleToleranceRadians();

  // Build sweep2 args...
  CArgsRhinoSweep2 args;

  // Rails
  CRhinoPolyEdge edge0, edge1;
  edge0.Create(rails[0]);
  edge1.Create(rails[1]);

  if (!RhinoDoCurveDirectionsMatch(&edge0, &edge1))
    edge1.Reverse();

  args.m_rail_curves[0] = &edge0;
  args.m_rail_curves[1] = &edge1;

  args.m_rail_pick_points[0] = ON_UNSET_POINT;
  args.m_rail_pick_points[1] = ON_UNSET_POINT;

  bool bClosedRails = (args.m_rail_curves[0]->IsClosed() && args.m_rail_curves[1]->IsClosed());
  if (bClosedRails && bClosed)
    args.m_bClosed = bClosed;

  // Shapes
  for (i = 0; i < curves.Count(); i++)
  {
    const ON_Curve* crv = curves[i];
    if (crv)
    {
      args.m_shape_curves.Append(crv);

      double t0 = 0.0;
      if (!GetShapeParameterOnRail(*crv, edge0, tol, t0))
        return false;
      args.m_rail_params[0].Append(t0);

      double t1 = 0.0;
      if (!GetShapeParameterOnRail(*crv, edge1, tol, t1))
        return false;
      args.m_rail_params[1].Append(t1);
    }
  }

  // Points
  if (start_pt)
    args.m_points.Append(*start_pt);
  if (end_pt)
    args.m_points.Append(*end_pt);

  // Style
  args.m_bSameHeight = bSameHeight;
  args.m_bSimpleSweep = bSimpleSweep;

  // Simplify
  args.m_simplify = RHINO_CLAMP(simplify, 0, 2); // 0 == none, 1 == refit, 2 == rebuild
  args.m_rebuild_count = rebuild_count;
  args.m_refit_tolerance = tol;

  // Other
  args.m_sweep_tolerance = tol;
  args.m_angle_tolerance = angtol;

  // Calculate sweep
  BOOL rc = RhinoSweep2(args, out_breps);

  return (rc && 0 != out_breps.Count());
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleSweep2Point : public CRhinoCommand
{
public:
  CCommandSampleSweep2Point() {}
  ~CCommandSampleSweep2Point() {}
  UUID CommandUUID()
  {
    // {28A4216-C489-41BE-95C4-67B6068C3314}
    static const GUID SampleSweep2PointCommand_UUID =
    { 0x28A4216, 0xC489, 0x41BE, { 0x95, 0xC4, 0x67, 0xB6, 0x06, 0x8C, 0x33, 0x14 } };
    return SampleSweep2PointCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSweep2Point"; }
  const wchar_t* LocalCommandName() { return L"SampleSweep2Point"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSweep2Point object
static class CCommandSampleSweep2Point theSampleSweep2PointCommand;

CRhinoCommand::result CCommandSampleSweep2Point::RunCommand(const CRhinoCommandContext& context)
{
  // Select first rail curve
  CRhinoGetObject go0;
  go0.SetCommandPrompt(L"Select first rail");
  go0.SetGeometryFilter(CRhinoGetObject::curve_object);
  go0.EnablePreSelect(false);
  go0.GetObjects(1, 1);
  if (go0.CommandResult() != CRhinoCommand::success)
    return go0.CommandResult();

  const ON_Curve* rail0 = go0.Object(0).Curve();
  if (0 == rail0)
    return CRhinoCommand::failure;

  // Select second rail curve
  CRhinoGetObject go1;
  go1.SetCommandPrompt(L"Select second rail");
  go1.SetGeometryFilter(CRhinoGetObject::curve_object);
  go1.EnablePreSelect(false);
  go1.EnableDeselectAllBeforePostSelect(false);
  go1.GetObjects(1, 1);
  if (go1.CommandResult() != CRhinoCommand::success)
    return go1.CommandResult();

  const ON_Curve* rail1 = go1.Object(0).Curve();
  if (0 == rail1)
    return CRhinoCommand::failure;

  // Select cross section curve
  CRhinoGetObject gc;
  gc.SetCommandPrompt(L"Select cross section");
  gc.SetGeometryFilter(CRhinoGetObject::curve_object);
  gc.EnablePreSelect(false);
  gc.EnableDeselectAllBeforePostSelect(false);
  gc.GetObjects(1, 1);
  if (gc.CommandResult() != CRhinoCommand::success)
    return gc.CommandResult();

  const ON_Curve* crv = gc.Object(0).Curve();
  if (0 == crv)
    return CRhinoCommand::failure;

  // Select end point
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"End point");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint end_pt = gp.Point();

  // Here we go...
  const ON_Curve* rails[2];
  rails[0] = rail0;
  rails[1] = rail1;

  ON_SimpleArray<const ON_Curve*> curves;
  curves.Append(crv);

  // Call RhinoSweep2Helper
  ON_SimpleArray<ON_Brep*> out_breps;
  bool rc = RhinoSweep2Helper(
    context.m_doc,
    rails,
    curves,
    out_breps,
    0,
    &end_pt
  );

  if (rc)
  {
    CRhinoObjectAttributes attribs;
    context.m_doc.GetDefaultObjectAttributes(attribs);
    attribs.m_wire_density = -1;

    // Add results to doucment
    for (int i = 0; i < out_breps.Count(); i++)
    {
      ON_Brep* brep = out_breps[i];
      if (brep)
      {
        CRhinoBrepObject* brep_obj = new CRhinoBrepObject(attribs);
        brep_obj->SetBrep(brep);
        brep = 0;
        context.m_doc.AddObject(brep_obj);
      }
    }

    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleSweep2Point command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
