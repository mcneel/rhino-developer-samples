using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsCreateBooleanRegions : Command
  {
    public override string EnglishName => "SampleCsCreateBooleanRegions";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var all = false;
      var combine = false;

      var all_option = new OptionToggle(all, "No", "Yes");
      var combine_option = new OptionToggle(combine, "No", "Yes");
      var res = GetResult.Nothing;

      var go = new GetObject();
      go.SetCommandPrompt("Select curves");
      go.GeometryFilter = ObjectType.Curve;
      go.EnablePreSelect(false, true);
      for (;;)
      {
        go.ClearCommandOptions();
        go.AddOptionToggle("AllRegions", ref all_option);
        go.AddOptionToggle("CombineRegions", ref combine_option);
        res = go.GetMultiple(1, 0);
        if (res != GetResult.Option)
          break;
      }

      if (res != GetResult.Object)
        return Result.Cancel;

      var curves = new List<Curve>(go.ObjectCount);
      foreach (var obj_ref in go.Objects())
      {
        var curve = obj_ref.Curve();
        if (null == curve)
          return Result.Failure;
        curves.Add(curve);
      }

      all = all_option.CurrentValue;
      combine = combine_option.CurrentValue;

      var points = new List<Point3d>();
      if (!all)
      {
        var gp = new GetPoint();
        gp.SetCommandPrompt("Pick region points.  Press <Enter> when done");
        gp.AcceptNothing(true);
        for (;;)
        {
          res = gp.Get();
          if (res != GetResult.Point)
            break;
          points.Add(gp.Point());
        }
      }

      var plane = Plane.WorldXY;
      var tolerance = doc.ModelAbsoluteTolerance;
      var regions = all
        ? Curve.CreateBooleanRegions(curves, plane, combine, tolerance)
        : Curve.CreateBooleanRegions(curves, plane, points, combine, tolerance);

      if (null == regions)
        return Result.Failure;

      for (var i = 0; i < regions.RegionCount; i++)
      {
        var boundaries = regions.RegionCurves(i);
        foreach (var boundary in boundaries)
          doc.Objects.AddCurve(boundary);
      }

      if (!all)
      {
        for (var i = 0; i < regions.PointCount; i++)
        {
          var point_index = regions.RegionPointIndex(i);
          if (point_index >= 0)
            doc.Objects.AddPoint(points[point_index]);
        }
      }

      regions.Dispose();

      foreach (var obj_ref in go.Objects())
        doc.Objects.Delete(obj_ref, false, false);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
