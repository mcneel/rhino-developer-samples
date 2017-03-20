using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;
using Rhino.Input;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsClassifyCurve command
  /// </summary>
  [System.Runtime.InteropServices.Guid("2dc99333-d983-464f-a0c1-f922ec7efc63")]
  public class SampleCsClassifyCurve : Command
  {
    private double m_tolerance;
    private double m_angle_tolerance;

    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsClassifyCurve()
    {
    }

    /// <summary>
    /// EnglishName override
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsClassifyCurve"; }
    }

    /// <summary>
    /// RunCommand override
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef object_ref;
      Result rc = RhinoGet.GetOneObject("Select curve to classify", false, ObjectType.Curve, out object_ref);
      if (rc != Result.Success)
        return rc;

      Curve curve = object_ref.Curve();
      if (null == curve)
        return Result.Failure;

      m_tolerance = doc.ModelAbsoluteTolerance;
      m_angle_tolerance = doc.ModelAngleToleranceRadians;

      // Is the curve a line?
      if (IsLine(curve))
      {
        RhinoApp.WriteLine("Curve is a line.");
        return Result.Success;
      }

      // Is the curve an arc or circle?
      bool bCircle = false;
      if (IsArc(curve, ref bCircle))
      {
        if (bCircle)
          RhinoApp.WriteLine("Curve is a circle.");
        else
          RhinoApp.WriteLine("Curve is an arc.");
        return Result.Success;
      }

      // Is the curve an ellipse or an elliptical arc?
      bool bEllipticalArc = false;
      if (IsEllipse(curve, ref bEllipticalArc))
      {
        if (bEllipticalArc)
          RhinoApp.WriteLine("Curve is an elliptical arc.");
        else
          RhinoApp.WriteLine("Curve is an ellipse.");
        return Result.Success;
      }

      // Is the curve some kind of polyline?
      int point_count = 0;     // number of points (vertices)
      bool bClosed = false;    // curve is closed
      bool bPlanar = false;    // curve is planar
      bool bLength = false;    // segment lengths are identical
      bool bAngle = false;     // angles between segments are identical
      bool bIntersect = false; // curve self-intersects
      if (IsPolyline(curve, ref point_count, ref bClosed, ref bPlanar, ref bLength, ref bAngle, ref bIntersect))
      {
        // If the curve is closed, planar, and does not self-intersect, then it must be a polygon.
        if (bClosed && bPlanar && !bIntersect)
        {
          // If segment distances and angles are identical, then it must be a regular polygon.
          if (bLength && bAngle)
          {
            switch (point_count)
            {
              case 3:
                RhinoApp.WriteLine("Curve is an equilateral triangle.");
                break;
              case 4:
                RhinoApp.WriteLine("Curve is a square.");
                break;
              case 5:
                RhinoApp.WriteLine("Curve is a regular pentagon.");
                break;
              case 6:
                RhinoApp.WriteLine("Curve is a regular hexagon.");
                 break;
              case 7:
                RhinoApp.WriteLine("Curve is a regular heptagon.");
                break;
              case 8:
                  RhinoApp.WriteLine("Curve is a regular octagon.");
                break;
              case 9:
                  RhinoApp.WriteLine("Curve is a regular nonagon.");
                break;
              case 10:
                RhinoApp.WriteLine("Curve is a regular decagon.");
                break;
              default:
                RhinoApp.WriteLine("Curve is a regular polygon.");
                break;
            }
          }

          // If segment distances or angles are not identical, then it must be an irregular polygon.
          else
          {
            switch (point_count)
            {
              case 3:
                // I'll leave searching for isosceles, scalene, acute, obtuse and right triangles for somebody else...
                RhinoApp.WriteLine("Curve is an triangle.");
                break;
              case 4:
              {
                // I'll leave searching for parallelogram and trapezoids for somebody else...
                if (bAngle)
                  RhinoApp.WriteLine("Curve is a rectangle.");
                else if (bLength)
                  RhinoApp.WriteLine("Curve is a rhombus.");
                else
                  RhinoApp.WriteLine("Curve is a quadrilateral.");
                break;
              }
              case 5:
                RhinoApp.WriteLine("Curve is a irregular pentagon.");
                break;
              case 6:
                RhinoApp.WriteLine("Curve is a irregular hexagon.");
                break;
              case 7:
                RhinoApp.WriteLine("Curve is a irregular heptagon.");
                break;
              case 8:
                RhinoApp.WriteLine("Curve is a irregular octagon.");
                break;
              case 9:
                RhinoApp.WriteLine("Curve is a irregular nonagon.");
                break;
              case 10:
                RhinoApp.WriteLine("Curve is a irregular decagon.");
                break;
              default:
                RhinoApp.WriteLine("Curve is a irregular polygon.");
                break;
            }
          }
        }
        else
        {
          RhinoApp.WriteLine("Curve is a polyline.");
        }

        return Result.Success;
      }

      // Is the curve a polycurve?
      if (IsPolyCurve(curve))
      {
        RhinoApp.WriteLine("Curve is a polycurve.");
        return Result.Success;
      }

      // Is the curve a NURBS curve?
      if (IsNurbsCurve(curve))
      {
        RhinoApp.WriteLine("Curve is a NURBS curve.");
        return Result.Success;
      }

      // Give up...
      RhinoApp.WriteLine("Curve is unclassified.");

      return Result.Success;
    }

    /// <summary>
    /// Test for a curve that is a line (or looks like a line).
    /// </summary>
    private bool IsLine(Curve curve)
    {
      if (null == curve)
        return false;

      // Is the curve a line curve?
      LineCurve line_curve = curve as LineCurve;
      if (null != line_curve)
        return true;

      // Is the curve a polyline that looks like a line?
      // (Should never need to test for this...)
      PolylineCurve polyline_curve = curve as PolylineCurve;
      if (null != polyline_curve && 2 == polyline_curve.PointCount)
        return true;

      // Is the curve a polycurve that looks like a line?
      // (Should never need to test for this...)
      PolyCurve poly_curve = curve as PolyCurve;
      if (null != poly_curve)
      {
        Polyline polyline;
        if (poly_curve.TryGetPolyline(out polyline))
        {
          if (2 == polyline.Count)
            return true;
        }
      }

      // Is the curve a NURBS curve that looks like a line?
      NurbsCurve nurbs_curve = curve as NurbsCurve;
      if (null != nurbs_curve)
      {
        Polyline polyline;
        if (nurbs_curve.TryGetPolyline(out polyline))
        {
          if (2 == polyline.Count)
            return true;
        }
      }

      return false;
    }

    /// <summary>
    /// Test for a curve that is an arc (or looks like an arc).
    /// </summary>
    private bool IsArc(Curve curve, ref bool bCircle)
    {
      if (null == curve)
        return false;

      // Is the curve an arc curve?
      ArcCurve arc_curve = curve as ArcCurve;
      if (null != arc_curve)
      {
        bCircle = arc_curve.IsCompleteCircle;
        return true;
      }

      // Is the curve a polycurve that looks like an arc?
      // (Should never need to test for this...)
      PolyCurve poly_curve = curve as PolyCurve;
      if (null != poly_curve)
      {
        Arc arc;
        if (poly_curve.TryGetArc(out arc))
        {
          bCircle = arc.IsCircle;
          return true;
        }
      }

      // Is the curve a NURBS curve that looks like an arc?
      NurbsCurve nurbs_curve = curve as NurbsCurve;
      if (null != nurbs_curve)
      {
        Arc arc;
        if (nurbs_curve.TryGetArc(out arc))
        {
          bCircle = arc.IsCircle;
          return true;
        }
      }

      return false;
    }

    /// <summary>
    /// Test for a curve that looks like an ellipse.
    /// </summary>
    private bool IsEllipse(Curve curve, ref bool bEllipticalArc)
    {
      if (null == curve)
        return false;

      // Is the curve a polycurve that looks like an ellipse?
      // (Should never need to test for this...)
      PolyCurve poly_curve = curve as PolyCurve;
      if (null != poly_curve)
      {
        Ellipse ellipse;
        if (poly_curve.TryGetEllipse(out ellipse))
        {
          bEllipticalArc = !ellipse.ToNurbsCurve().IsClosed;
          return true;
        }
      }

      // Is the curve a NURBS curve that looks like an ellipse?
      NurbsCurve nurbs_curve = curve as NurbsCurve;
      if (null != nurbs_curve)
      {
        Ellipse ellipse;
        if (nurbs_curve.TryGetEllipse(out ellipse))
        {
          bEllipticalArc = !nurbs_curve.IsClosed;
          return true;
        }
      }

      return false;
    }

    /// <summary>
    /// Test for a curve that is a polyline (or looks like a polyline).
    /// </summary>
    bool IsPolyline(Curve curve, ref int point_count, ref bool bClosed, ref bool bPlanar, ref bool bLength, ref bool bAngle, ref bool bIntersect)
    {
      if (null == curve)
        return false;

      List<Point3d> points = new List<Point3d>();

      // Is the curve a polyline curve?
      PolylineCurve polyline_curve = curve as PolylineCurve;
      if (null != polyline_curve)
      {
        if (polyline_curve.PointCount <= 2)
          return false;

        for (int i = 0; i < polyline_curve.PointCount; i++)
          points.Add(polyline_curve.Point(i));
      }

      // Is the curve a polycurve that looks like an polyline?
      PolyCurve poly_curve = curve as PolyCurve;
      if (null != poly_curve)
      {
        Polyline polyline;
        if (poly_curve.TryGetPolyline(out polyline))
        {
          if (polyline.Count <= 2)
            return false;

          for (int i = 0; i < polyline.Count; i++)
            points.Add(polyline[i]);
        }
      }

      // Is the curve a NURBS curve that looks like an polyline?
      NurbsCurve nurbs_curve = curve as NurbsCurve;
      if (null != nurbs_curve)
      {
        Polyline polyline;
        if (nurbs_curve.TryGetPolyline(out polyline))
        {
          if (polyline.Count <= 2)
            return false;

          for (int i = 0; i < polyline.Count; i++)
            points.Add(polyline[i]);
        }
      }

      if (0 == points.Count)
        return false;

      // Is the curve closed?
      bClosed = curve.IsClosed;

      // Is the curve planar?
      bPlanar = curve.IsPlanar();

      // Get the point (vertex) count.
      point_count = (bClosed ? points.Count - 1 : points.Count);

      // Test for self-intersection.
      CurveIntersections intesections = Intersection.CurveSelf(curve, m_tolerance);
      bIntersect = (intesections.Count > 0) ? true : false;

      // If the curve is not closed, no reason to continue...
      if (!bClosed)
        return true;

      // Test if the distance between each point is identical.
      double distance = 0.0;
      for (int i = 1; i < points.Count; i++)
      {
        Point3d p0 = points[i - 1];
        Point3d p1 = points[i];
        Vector3d v = p0 - p1;

        double d = v.Length;
        if (i == 1)
        {
          distance = d;
          continue;
        }
        else if (Math.Abs(distance - d) < m_tolerance)
        {
          continue;
        }
        else
        {
          distance = RhinoMath.UnsetValue;
          break;
        }
      }

      // Set return value.
      bLength = RhinoMath.IsValidDouble(distance);

      // Test if the angle between each point is identical.
      double angle = 0.0;
      for (int i = 1; i < points.Count - 1; i++)
      {
        Point3d p0 = points[i - 1];
        Point3d p1 = points[i];
        Point3d p2 = points[i + 1];

        Vector3d v0 = p1 - p0;
        Vector3d v1 = p1 - p2;

        v0.Unitize();
        v1.Unitize();

        double a = Vector3d.VectorAngle(v0, v1);
        if (i == 1)
        {
          angle = a;
          continue;
        }
        else if (Math.Abs(angle - a) < m_angle_tolerance)
        {
          continue;
        }
        else
        {
          angle = RhinoMath.UnsetValue;
          break;
        }
      }

      // Set return value.
      bAngle =  RhinoMath.IsValidDouble(angle);

      return true;
    }

    /// <summary>
    /// Test for a curve that is a polycurve.
    /// </summary>
    private bool IsPolyCurve(Curve curve)
    {
      if (null == curve)
        return false;

      PolyCurve poly_curve = curve as PolyCurve;
      if (null != poly_curve)
        return true;

      return false;
    }
 
    /// <summary>
    /// Test for a curve that is a NURBS curve.
    /// </summary>
    private bool IsNurbsCurve(Curve curve)
    {
      if (null == curve)
        return false;

      NurbsCurve nurbs_curve = curve as NurbsCurve;
      if (null != nurbs_curve)
        return true;

      return false;
    }
  }
}
