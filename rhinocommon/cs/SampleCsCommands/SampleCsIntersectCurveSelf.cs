using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsIntersectCurveSelf : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsIntersectCurveSelf"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select curve for self-intersection test");
      go.GeometryFilter = ObjectType.Curve;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curve = go.Object(0).Curve();
      if (null == curve)
        return Result.Failure;

      var tolerance = doc.ModelAbsoluteTolerance;
      var ccx_events = Intersection.CurveSelf(curve, tolerance);

      foreach (var ccx in ccx_events)
      {
        var rhobject_id = Guid.Empty;
        if (ccx.IsPoint)
          rhobject_id = doc.Objects.AddPoint(ccx.PointA);
        else if (ccx.IsOverlap)
        {
          var trim_curve = curve.Trim(ccx.OverlapA);
          if (null != trim_curve)
            rhobject_id = doc.Objects.AddCurve(trim_curve);
        }

        if (rhobject_id != Guid.Empty)
        {
          var rhobject = doc.Objects.Find(rhobject_id);
          if (null != rhobject)
            rhobject.Select(true);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
