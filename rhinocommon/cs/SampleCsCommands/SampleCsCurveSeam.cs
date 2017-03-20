using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;
using Rhino.Geometry;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("e9e0feff-c684-429b-8e63-2f3e5dc3577f")]
  public class SampleCsCurveSeam : Command
  {
    public SampleCsCurveSeam()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsCurveSeam"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select closed curve for seam adjustment");
      go.GeometryFilter = ObjectType.Curve;
      go.GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve;
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      Curve curve = go.Object(0).Curve();
      if (null == curve)
        return Result.Failure;

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("New seam location");
      gp.Constrain(curve, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      Point3d point = gp.Point();

      Curve new_curve = curve.DuplicateCurve();
      double t = Rhino.RhinoMath.UnsetValue;
      if (new_curve.ClosestPoint(point, out t))
      {
        if (new_curve.ChangeClosedCurveSeam(t))
        {
          doc.Objects.Replace(go.Object(0), new_curve);
        }
      }

      return Result.Success;
    }
  }
}
