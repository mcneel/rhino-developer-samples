using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("ae164ad5-62e1-4d1f-995c-3eff15262bb6")]
  public class SampleCsDeleteSubCurve : Command
  {
    public SampleCsDeleteSubCurve()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsDeleteSubCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;

      const string prompt = "Select curve to edit";
      const ObjectType object_type = ObjectType.Curve;

      Result res = RhinoGet.GetOneObject(prompt, false, object_type, out obj_ref);
      if (res != Result.Success)
        return res;

      Curve crv = obj_ref.Curve();
      if (null == crv)
        return Result.Failure;

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Start point for deletion");
      gp.Constrain(crv, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      double t0 = RhinoMath.UnsetValue;
      Curve out_crv = gp.PointOnCurve(out t0);
      if (null == out_crv)
        return Result.Failure;

      gp.SetCommandPrompt("End point for deletion");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      double t1 = RhinoMath.UnsetValue;
      out_crv = gp.PointOnCurve(out t1);
      if (null == out_crv)
        return Result.Failure;

      if (Math.Abs(t1 - t0) < RhinoMath.ZeroTolerance)
        return Result.Nothing;
  
      Interval range = new Interval(t0, t1);
      if (!crv.IsClosed && range.IsDecreasing)
        range.Swap();

      Curve sub_crv = crv.Trim(crv.Domain.Min, range.Min);
      if (null != sub_crv)
        doc.Objects.AddCurve(sub_crv);

      sub_crv = crv.Trim(range.Max, crv.Domain.Max);
      if (null != sub_crv)
        doc.Objects.AddCurve(sub_crv);

      doc.Objects.Delete(obj_ref, true);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
