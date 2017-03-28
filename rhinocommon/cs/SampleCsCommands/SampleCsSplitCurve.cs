using System;
using System.Collections.Generic;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("ec8f64cd-0599-40eb-b495-5cb763068b6e")]
  public class SampleCsSplitCurve : Command
  {
    public SampleCsSplitCurve()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsSplitCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select curve to split");
      go.GeometryFilter = ObjectType.Curve;
      go.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      ObjRef object_ref = go.Object(0);
      RhinoObject rh_object = object_ref.Object();
      Curve curve = object_ref.Curve();
      if (null == rh_object || null == curve)
        return Result.Failure;

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Point on curve to split at");
      gp.Constrain(curve, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      Point3d point = gp.Point();
      double t = Rhino.RhinoMath.UnsetValue;
      if (!curve.ClosestPoint(point, out t))
        return Result.Failure;

      List<double> curve_t = new List<double>(3);
      curve_t.Add(curve.Domain.Min);
      curve_t.Add(curve.Domain.Max);
      curve_t.Add(t);
      curve_t.Sort();

      List<double> culled_t = curve_t.Distinct().ToList();
      if (culled_t.Count < 3)
        return Result.Nothing;

      ObjectAttributes attributes = rh_object.Attributes.Duplicate();
      attributes.ObjectId = Guid.Empty;

      for (int i = 0; i < culled_t.Count - 1; i++)
      {
        Interval domain = new Interval(culled_t[i], culled_t[i+1]);
        if (curve.Domain.IncludesInterval(domain))
        {
          Curve trim = curve.Trim(domain);
          if (null != trim)
            doc.Objects.Add(trim, attributes);
        }
      }

      doc.Objects.Delete(object_ref, false);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
