using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("fc2dcb30-1702-49af-80c0-ec4937300de3")]
  public class SampleCsEditPolyline : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsEditPolyline"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetPolylineCurve();
      go.SetCommandPrompt("Select polyline");
      go.GeometryFilter = ObjectType.Curve;
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Get underlying curve geometry
      var polyline_curve = go.Object(0).Geometry() as PolylineCurve;
      if (null == polyline_curve)
        return Result.Failure;

      // Make a copy of the geometry
      var new_polyline_curve = polyline_curve.DuplicateCurve() as PolylineCurve;
      if (null == new_polyline_curve)
        return Result.Failure;

      // Modify the geometry in some way
      for (var i = 0; i < new_polyline_curve.PointCount; i++)
      {
        var point = new_polyline_curve.Point(i);
        if (i%2 != 0)
        {
          point.X += 1;
          point.Y += 1;
        }
        else
        {
          point.X -= 1;
          point.Y -= 1;
        }
        new_polyline_curve.SetPoint(i, point);
      }

      // Replace the original object (geometry) with the modified one.
      doc.Objects.Replace(go.Object(0), new_polyline_curve);
      doc.Views.Redraw();

      return Result.Success;
    }
  }

  /// <summary>
  /// This GetObject-derived class will only select polyline curves.
  /// </summary>
  internal class GetPolylineCurve : GetObject
  {
    public override bool CustomGeometryFilter(RhinoObject obj, GeometryBase geom, ComponentIndex ci)
    {
      var polyline_curve = geom as PolylineCurve;
      return polyline_curve != null;
    }
  }

}
