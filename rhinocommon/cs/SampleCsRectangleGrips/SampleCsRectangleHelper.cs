using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Geometry;

namespace SampleCsRectangleGrips
{
  internal static class SampleCsRectangleHelper
  {
    /// <summary>
    /// Verifies a set of points is a rectangle
    /// </summary>
    public static bool IsRectangle(IEnumerable<Point3d> points)
    {
      var curve = new PolylineCurve(points);
      return curve.IsValid && IsRectangle(curve);
    }

    /// <summary>
    /// Verifies a Polyline is a rectangle
    /// </summary>
    public static bool IsRectangle(Polyline polyline)
    {
      if (polyline == null)
        return false;

      var curve = new PolylineCurve(polyline);
      return curve.IsValid && IsRectangle(curve);
    }

    /// <summary>
    /// Verifies a PolylineCurve is a rectangle
    /// </summary>
    public static bool IsRectangle(PolylineCurve curve)
    {
      // Curve should be a valid, closed, planar polyline curve with 5 points
      if (curve == null || !curve.IsValid || !curve.IsClosed || !curve.IsPlanar() || curve.PointCount != 5)
        return false;

      // Angle between each segment should be 90 degrees
      const double angle = 90.0 * (Math.PI / 180.0);
      for (var i = 1; i < curve.PointCount - 1; i++)
      {
        var p0 = curve.Point(i - 1);
        var p1 = curve.Point(i);
        var p2 = curve.Point(i + 1);

        var v0 = p1 - p0;
        v0.Unitize();

        var v1 = p1 - p2;
        v1.Unitize();

        var a = Vector3d.VectorAngle(v0, v1);
        if (Math.Abs(angle - a) >= RhinoMath.DefaultAngleTolerance)
          return false;
      }

      return true;
    }

    /// <summary>
    /// Verifies some geometry is a rectangle
    /// </summary>
    public static bool IsRectangle(GeometryBase geometry)
    {
      var polyline_curve = geometry as PolylineCurve;
      return polyline_curve != null && IsRectangle(polyline_curve);
    }
  }
}
