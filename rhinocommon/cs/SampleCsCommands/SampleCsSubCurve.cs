using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsSubCurve : Command
  {
    public SampleCsSubCurve()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsSubCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjRef obj_ref;
      Result rc = Rhino.Input.RhinoGet.GetOneObject("Select curve", false, Rhino.DocObjects.ObjectType.Curve, out obj_ref);
      if (rc != Result.Success)
        return rc;

      Rhino.DocObjects.RhinoObject obj = obj_ref.Object();
      if (null == obj)
        return Result.Failure;

      obj_ref.Object().Select(false);
      doc.Views.Redraw();

      Rhino.Geometry.Curve crv = obj_ref.Curve();
      if (null == crv)
        return Result.Failure;

      Rhino.Input.Custom.GetPoint gp = new Rhino.Input.Custom.GetPoint();
      gp.SetCommandPrompt("First point on curve");
      gp.Constrain(crv, false);
      gp.Get();
      rc = gp.CommandResult();
      if (rc != Result.Success)
        return rc;

      double t0 = Rhino.RhinoMath.UnsetValue;
      if (null == gp.PointOnCurve(out t0))
        return Result.Failure;

      gp.SetCommandPrompt("Second point on curve");
      gp.Get();
      rc = gp.CommandResult();
      if (rc != Result.Success)
        return rc;

      double t1 = Rhino.RhinoMath.UnsetValue;
      if (null == gp.PointOnCurve(out t1))
        return Result.Failure;

      if (System.Math.Abs(t1-t0) < Rhino.RhinoMath.ZeroTolerance)
        return Result.Failure;

      if (crv.IsClosed || (!crv.IsClosed && t0 > t1))
      {
        double t = t0; 
        t0 = t1; 
        t1 = t;
      }

      Rhino.Geometry.Interval range = new Rhino.Geometry.Interval(t0, t1);
      Rhino.Geometry.Curve subcrv = crv.Trim(range);
      if (null != subcrv)
      {
        System.Guid id = doc.Objects.Add(subcrv);
        obj = doc.Objects.Find(id);
        if (null != obj)
          obj.Select(true);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
