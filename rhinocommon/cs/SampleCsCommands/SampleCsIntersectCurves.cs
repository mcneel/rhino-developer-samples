using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;
using System;

namespace SampleCsCommands
{
  public class SampleCsIntersectCurves : Command
  {
    public override string EnglishName => "SampleCsIntersectCurves";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select two curves for intersection test");
      go.GeometryFilter = ObjectType.Curve;
      go.GetMultiple(2, 2);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      Rhino.Geometry.Curve curve0 = go.Object(0).Curve();
      Rhino.Geometry.Curve curve1 = go.Object(1).Curve();
      if (null == curve0 || null == curve1)
        return Result.Failure;

      double tolerance = doc.ModelAbsoluteTolerance;
      CurveIntersections ccx_events = Intersection.CurveCurve(curve0, curve1, tolerance, tolerance);
      foreach (IntersectionEvent ccx in ccx_events)
      {
        Guid rhobject_id = Guid.Empty;
        if (ccx.IsPoint)
          rhobject_id = doc.Objects.AddPoint(ccx.PointA);
        else if (ccx.IsOverlap)
        {
          Rhino.Geometry.Curve curve = curve0.Trim(ccx.OverlapA);
          if (null != curve)
            rhobject_id = doc.Objects.AddCurve(curve);
        }

        if (rhobject_id != Guid.Empty)
        {
          RhinoObject rhobject = doc.Objects.Find(rhobject_id);
          if (null != rhobject)
            rhobject.Select(true);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
