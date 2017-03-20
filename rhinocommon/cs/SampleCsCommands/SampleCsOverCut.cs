using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsOverCut command
  /// </summary>
  [System.Runtime.InteropServices.Guid("e9aa0945-78f5-4165-b368-865c7d92bad7")]
  public class SampleCsOverCut : Command
  {
    enum CutType
    {
      Corner = 0,
      Bottom = 1,
      Side = 2,
      BottomAndSide = 3
    }

    private const double RADIUS = 1.0;
    private const CutType CUTTYPE = CutType.Corner;
    private double Tolerance { get; set; }

    /// <summary>
    /// EnglishName
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsOverCut"; }
    }

    /// <summary>
    /// RunCommand
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Tolerance = doc.ModelAbsoluteTolerance;

      // Get persistent settings
      var settings = Settings;
      var radius = settings.GetDouble("Radius", RADIUS);
      var cut_type = settings.GetEnumValue("CutType", CUTTYPE);

      // Select closed,planar curve
      var go = new GetClosedPlanarPolyline(Tolerance);
      go.SetCommandPrompt("Select closed, planar polyline");
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Get curve
      var obj_ref = go.Object(0);
      var curve = obj_ref.Curve();
      if (null == curve || !curve.IsClosed || !curve.IsPlanar())
        return Result.Failure;

      // Get polyline
      Polyline polyline;
      if (!curve.TryGetPolyline(out polyline))
        return Result.Failure;

      // Since first and last point are the same, remove the last point.
      polyline.RemoveAt(polyline.Count - 1);

      // Get curve plane
      Plane plane;
      if (!curve.TryGetPlane(out plane, Tolerance))
        return Result.Failure;

      // Get corner point indices
      var indices = FindInnerCornerPoints(curve, polyline, plane);
      if (0 == indices.Length)
      {
        RhinoApp.WriteLine("No inner corners found.");
        return Result.Nothing;
      }

      // Show preview conduit
      var conduit = new SampleCsOverCutConduit
      {
        Circles = CalculateCuttingCircles(curve, polyline, indices, plane, radius, cut_type),
        Enabled = true
      };
      doc.Views.Redraw();

      Result rc;

      // Choose overcut options
      var gp = new GetOption();
      gp.SetCommandPrompt("Choose overcut option");
      gp.AcceptNothing(true);
      for (;;)
      {
        gp.ClearCommandOptions();
        var cut_type_index = gp.AddOptionEnumList("CutType", cut_type);
        var radius_option = new OptionDouble(radius, true, Tolerance);
        var radius_index = gp.AddOptionDouble("Radius", ref radius_option);
        var res = gp.Get();

        if (res == GetResult.Option)
        {
          var option = gp.Option();
          if (null != option)
          {
            if (option.Index == cut_type_index)
            {
              var list = Enum.GetValues(typeof (CutType)).Cast<CutType>().ToList();
              cut_type = list[option.CurrentListOptionIndex];
            }
            else if (option.Index == radius_index)
            {
              radius = radius_option.CurrentValue;
            }

            conduit.Circles = CalculateCuttingCircles(curve, polyline, indices, plane, radius, cut_type);
            doc.Views.Redraw();
          }
          continue;
        }

        if (res == GetResult.Nothing)
        {
          rc = Result.Success;
          break;
        }

        rc = Result.Cancel;
        break;
      }

      conduit.Enabled = false;

      if (rc == Result.Success)
      {
        // Try differencing circles from curve
        var success = true;
        var new_curve = curve;
        for (var i = 0; i < conduit.Circles.Count && success; i++)
        {
          var new_curves = Curve.CreateBooleanDifference(new_curve, new ArcCurve(conduit.Circles[i]));
          if (1 == new_curves.Length && null != new_curves[0])
            new_curve = new_curves[0];
          else
            success = false;
        }

        // Add geometry to document
        if (success && null != new_curve)
        {
          doc.Objects.Replace(obj_ref, new_curve);
        }
        else
        {
          for (var i = 0; i < conduit.Circles.Count; i++)
            doc.Objects.AddCircle(conduit.Circles[i]);
        }

        // Set persistent settings
        settings.SetDouble("Radius", radius);
        settings.SetEnumValue("CutType", cut_type);
      }

      doc.Views.Redraw();

      return rc;
    }

    /// <summary>
    /// FindInnerCornerPoints
    /// </summary>
    private int[] FindInnerCornerPoints(Curve curve, Polyline polyline, Plane plane)
    {
      if (null == curve || null == polyline)
        return new int[0];

      var indices = new List<int>();

      for (var current = 0; current < polyline.Count; current++)
      {
        var prev = (current == 0) ? polyline.Count - 1 : current - 1;
        var dir = polyline[current] - polyline[prev];
        dir.Unitize();
        dir *= 2.1*Tolerance;
        var point = polyline[current] + dir;
        if (PointContainment.Inside == curve.Contains(point, plane, Tolerance))
          indices.Add(current);
      }

      return indices.ToArray();
    }

    /// <summary>
    /// CalculateCuttingCircles
    /// </summary>
    private List<Circle> CalculateCuttingCircles(Curve curve, Polyline polyline, int[] indices, Plane plane, double radius, CutType cutType)
    {
      var circles = new List<Circle>();

      if (null == curve || null == polyline || null == indices)
        return circles;

      for (var i = 0; i < indices.Length; i++)
      {
        var current = indices[i];
        int prev, next;
        if (current == 0)
        {
          prev = polyline.Count - 1;
          next = current + 1;
        }
        else if (current == polyline.Count - 1)
        {
          prev = current - 1;
          next = 0;
        }
        else
        {
          prev = current - 1;
          next = current + 1;
        }

        var dir0 = polyline[prev] - polyline[current];
        var dir1 = polyline[next] - polyline[current];

        if (cutType == CutType.Corner)
        {
          dir0.Unitize();
          dir1.Unitize();

          var dir = 0.5 * (dir0 + dir1);
          dir.Unitize();
          dir *= radius;

          var circle_plane = plane;
          circle_plane.Origin = polyline[current];
          var circle = new Circle(circle_plane, radius);

          var xform = Transform.Translation(dir);
          circle.Transform(xform);

          circles.Add(circle);
        }

        if (cutType == CutType.Bottom || cutType == CutType.BottomAndSide)
        {
          dir0.Unitize();
          dir1.Unitize();

          var dir = Vector3d.Unset;

          dir0 *= 2.1 * Tolerance;
          var point = polyline[prev] + dir0;
          if (PointContainment.Inside == curve.Contains(point, plane, Tolerance))
            dir = dir0;
          else
          {
            dir1 *= 2.1 * Tolerance;
            point = polyline[next] + dir1;
            if (PointContainment.Inside == curve.Contains(point, plane, Tolerance))
              dir = dir1;
          }

          if (dir.IsValid)
          {
            dir.Unitize();
            dir *= radius;

            var circle_plane = plane;
            circle_plane.Origin = polyline[current];
            var circle = new Circle(circle_plane, radius);

            var xform = Transform.Translation(dir);
            circle.Transform(xform);

            circles.Add(circle);
          }
        }

        if (cutType == CutType.Side || cutType == CutType.BottomAndSide)
        {
          dir0.Unitize();
          dir1.Unitize();

          var dir = Vector3d.Unset;

          dir0 *= 2.1 * Tolerance;
          var point = polyline[prev] + dir0;
          if (PointContainment.Inside != curve.Contains(point, plane, Tolerance))
            dir = dir0;
          else
          {
            dir1 *= 2.1 * Tolerance;
            point = polyline[next] + dir1;
            if (PointContainment.Inside != curve.Contains(point, plane, Tolerance))
              dir = dir1;
          }

          if (dir.IsValid)
          {
            dir.Unitize();
            dir *= radius;

            var circle_plane = plane;
            circle_plane.Origin = polyline[current];
            var circle = new Circle(circle_plane, radius);

            var xform = Transform.Translation(dir);
            circle.Transform(xform);

            circles.Add(circle);
          }
        }
      }

      return circles;
    }

    /// <summary>
    /// Finds the concave corner points in a closed, planar polyline curve.
    /// </summary>
    /// <param name="curve">The polyline curve.</param>
    /// <param name="tolerance">The tolerance required to determine if the curve is planar.
    /// When in doubt, use the document's model absolute tolerance.</param>
    /// <returns>The concave points if successful.</returns>
    private static Point3d[] FindConcaveCornerPoints(Curve curve, double tolerance)
    {
      var rc = new Point3d[0];

      // Make sure we have a curve and it's closed.
      if (null == curve || !curve.IsClosed)
        return rc;

      // Make sure the curve is planar, within our tolerance.
      Plane plane;
      if (!curve.TryGetPlane(out plane, tolerance))
        return rc;

      // Make sure the curve is a polyline.
      Polyline polyline;
      if (!curve.TryGetPolyline(out polyline) || null == polyline)
        return rc;

      // For closed polylines, the first and last points are identical.
      // So we can remove the last one.
      polyline.RemoveAt(polyline.Count - 1);

      // Find the concave corner points...
      var points = new List<Point3d>();
      for (var current = 0; current < polyline.Count; current++)
      {
        var prev = current == 0 ? polyline.Count - 1 : current - 1;
        var next = current == polyline.Count - 1 ? 0 : current + 1;

        var dir0 = polyline[current] - polyline[prev];
        dir0.Unitize();

        var dir1 = polyline[next] - polyline[current];
        dir1.Unitize();

        var dir2 = Vector3d.CrossProduct(dir0, dir1);
        dir2.Unitize();

        var dot = dir2 * plane.Normal; // dot product
        if (dot < 0.0)
          points.Add(polyline[current]);
      }

      return points.ToArray();
    }

  }


  /// <summary>
  /// OverCutConduit
  /// </summary>
  internal class SampleCsOverCutConduit : DisplayConduit
  {
    private Color TrackingColor { get; set; }
    public List<Circle> Circles { get; set; } 

    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsOverCutConduit()
    {
      TrackingColor = Rhino.ApplicationSettings.AppearanceSettings.TrackingColor;
      Circles = new List<Circle>();
    }

    /// <summary>
    /// DrawOverlay override
    /// </summary>
    protected override void DrawOverlay(DrawEventArgs e)
    {
      if (null != Circles)
      {
        for (var i = 0; i < Circles.Count; i++)
        {
          e.Display.DrawCircle(Circles[i], TrackingColor);
        }
      }
    }
  }


  /// <summary>
  /// GetClosedPlanarPolyline
  /// </summary>
  internal class GetClosedPlanarPolyline : GetObject
  {
    private double Tolerance { get; set; }

    /// <summary>
    /// Constructor
    /// </summary>
    public GetClosedPlanarPolyline(double tolerance)
    {
      Tolerance = tolerance;
    }

    /// <summary>
    /// CustomGeometryFilter override
    /// </summary>
    public override bool CustomGeometryFilter(RhinoObject rhinoObject, GeometryBase geometry, ComponentIndex componentIndex)
    {
      var curve = geometry as Curve;
      if (null == curve || !curve.IsClosed || !curve.IsPlanar(Tolerance))
        return false;

      Polyline polyline;
      if (!curve.TryGetPolyline(out polyline))
        return false;

      return true;
    }
  }

}
