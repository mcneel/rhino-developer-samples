using System;
using System.Collections.Generic;
using System.Linq;
using Rhino;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;

namespace examples_cs
{
  public class DivideCurveStraightCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csDivideCurveStraight"; }
    }

    private void NextintersectParamAndPoint(Curve[] overlapCurves, Point3d[] intersectPoints,
      Curve curve, out double intersectParam, out Point3d intersectPoint)
    {
      var intersect_params_and_points = new Dictionary<double, Point3d>();
      foreach (var point in intersectPoints)
      {
        double curve_param;
        curve.ClosestPoint(point, out curve_param);
        intersect_params_and_points[curve_param] = point;
      }
      foreach (var overlap_curve in overlapCurves)
      {
        intersect_params_and_points[overlap_curve.Domain.Min] = overlap_curve.PointAt(overlap_curve.Domain.Min);
        intersect_params_and_points[overlap_curve.Domain.Max] = overlap_curve.PointAt(overlap_curve.Domain.Max);
      }
      var min_t = intersect_params_and_points.Keys.Min();
      intersectParam = min_t;
      intersectPoint = intersect_params_and_points[intersectParam];
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // user input
      ObjRef[] obj_refs;
      var rc = RhinoGet.GetMultipleObjects("Select curve to divide", false, 
        ObjectType.Curve | ObjectType.EdgeFilter, out obj_refs);
      if (rc != Result.Success || obj_refs == null)
        return rc;
  
      double distance_between_divisions = 5;
      rc = RhinoGet.GetNumber("Distance between divisions", false, 
        ref distance_between_divisions, 1.0, Double.MaxValue);
      if (rc != Result.Success)
        return rc;


      // generate the points
      var points = new List<Point3d>();
      foreach (var obj_ref in obj_refs)
      {
        var curve = obj_ref.Curve();
        if (curve == null) return Result.Failure;

        var t0 = curve.Domain.Min;
        points.Add(curve.PointAt(t0));

        var sphere_center = curve.PointAt(t0);
        var t = t0;
        var rest_of_curve = curve;
        while (true)
        {
          var sphere = new Sphere(sphere_center, distance_between_divisions);
          Curve[] overlap_curves;
          Point3d[] intersect_points;
          var b = Intersection.CurveBrep(rest_of_curve, sphere.ToBrep(), 0.0, 
            out overlap_curves, out intersect_points);
          if (!b || (overlap_curves.Length == 0 && intersect_points.Length == 0))
            break;
          double intersect_param;
          Point3d intersect_point;
          NextintersectParamAndPoint(overlap_curves, intersect_points, rest_of_curve,
            out intersect_param, out intersect_point);
          points.Add(intersect_point);
          t = intersect_param;
          sphere_center = intersect_point;
          rest_of_curve = curve.Split(t)[1];
        }
      }
  
      foreach (var point in points)
        doc.Objects.AddPoint(point);
  
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}