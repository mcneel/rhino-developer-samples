using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsCreateBooleanRegions : Command
  {
    public override string EnglishName => "SampleCsCreateBooleanRegions";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      bool all = false;
      bool combine = false;

      OptionToggle all_option = new OptionToggle(all, "No", "Yes");
      OptionToggle combine_option = new OptionToggle(combine, "No", "Yes");
      GetResult res = GetResult.Nothing;

      GetObject go = new GetObject();
      go.SetCommandPrompt("Select curves");
      go.GeometryFilter = ObjectType.Curve;
      go.EnablePreSelect(false, true);
      for (; ; )
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

      List<Curve> curves = new List<Curve>(go.ObjectCount);
      foreach (ObjRef obj_ref in go.Objects())
      {
        Curve curve = obj_ref.Curve();
        if (null == curve)
          return Result.Failure;
        curves.Add(curve);
      }

      all = all_option.CurrentValue;
      combine = combine_option.CurrentValue;

      List<Point3d> points = new List<Point3d>();
      if (!all)
      {
        GetPoint gp = new GetPoint();
        gp.SetCommandPrompt("Pick region points.  Press <Enter> when done");
        gp.AcceptNothing(true);
        for (; ; )
        {
          res = gp.Get();
          if (res != GetResult.Point)
            break;
          points.Add(gp.Point());
        }
      }

      Plane plane = Plane.WorldXY;
      double tolerance = doc.ModelAbsoluteTolerance;
      CurveBooleanRegions regions = all
        ? Curve.CreateBooleanRegions(curves, plane, combine, tolerance)
        : Curve.CreateBooleanRegions(curves, plane, points, combine, tolerance);

      if (null == regions)
        return Result.Failure;

      for (int i = 0; i < regions.RegionCount; i++)
      {
        Curve[] boundaries = regions.RegionCurves(i);
        foreach (Curve boundary in boundaries)
          doc.Objects.AddCurve(boundary);
      }

      if (!all)
      {
        for (int i = 0; i < regions.PointCount; i++)
        {
          int point_index = regions.RegionPointIndex(i);
          if (point_index >= 0)
            doc.Objects.AddPoint(points[point_index]);
        }
      }

      regions.Dispose();

      foreach (ObjRef obj_ref in go.Objects())
        doc.Objects.Delete(obj_ref, false, false);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
