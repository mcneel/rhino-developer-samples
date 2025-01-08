using Rhino;
using Rhino.Geometry;
using System;
using System.Collections.Generic;

namespace SampleCsRectangleGrips
{
  internal static class SampleCsRectangleHelper
  {
    /// <summary>
    /// Verifies a set of points is a rectangle
    /// </summary>
    public static bool IsRectangle(IEnumerable<Point3d> points)
    {
      PolylineCurve curve = new PolylineCurve(points);
      return curve.IsValid && IsRectangle(curve);
    }

    /// <summary>
    /// Verifies a Polyline is a rectangle
    /// </summary>
    public static bool IsRectangle(Polyline polyline)
    {
      if (polyline == null)
        return false;

      PolylineCurve curve = new PolylineCurve(polyline);
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
      for (int i = 1; i < curve.PointCount - 1; i++)
      {
        Point3d p0 = curve.Point(i - 1);
        Point3d p1 = curve.Point(i);
        Point3d p2 = curve.Point(i + 1);

        Vector3d v0 = p1 - p0;
        v0.Unitize();

        Vector3d v1 = p1 - p2;
        v1.Unitize();

        double a = Vector3d.VectorAngle(v0, v1);
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
      PolylineCurve polyline_curve = geometry as PolylineCurve;
      return polyline_curve != null && IsRectangle(polyline_curve);
    }
  }
}
