using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("c7aeec14-7826-497b-943b-5332647b5165")]
  public class SampleCsIntersectCurves : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsIntersectCurves"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select two curves for intersection test");
      go.GeometryFilter = ObjectType.Curve;
      go.GetMultiple(2, 2);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curve0 = go.Object(0).Curve();
      var curve1 = go.Object(1).Curve();
      if (null == curve0 || null == curve1)
        return Result.Failure;

      var tolerance = doc.ModelAbsoluteTolerance;
      var ccx_events = Intersection.CurveCurve(curve0, curve1, tolerance, tolerance);
      foreach (var ccx in ccx_events)
      {
        var rhobject_id = Guid.Empty;
        if (ccx.IsPoint)
          rhobject_id = doc.Objects.AddPoint(ccx.PointA);
        else if (ccx.IsOverlap)
        {
          var curve = curve0.Trim(ccx.OverlapA);
          if (null != curve)
            rhobject_id = doc.Objects.AddCurve(curve);
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
