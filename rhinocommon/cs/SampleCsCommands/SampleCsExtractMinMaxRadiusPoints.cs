using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsExtractMinMaxRadiusPoints : Command
  {
    public override string EnglishName => "SampleCsExtractMinMaxRadiusPoints";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select curves to extract min/max radius points");
      go.GeometryFilter = ObjectType.Curve;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (var i = 0; i < go.ObjectCount; i++)
      {
        var curve = go.Object(i).Curve();
        if (null != curve)
        {
          var points = new List<Point3d>();
          if (GetMinMaxRadiusPoints(curve, points))
            doc.Objects.AddPoints(points);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    public static bool GetMinMaxRadiusPoints(Curve curve, List<Point3d> points)
    {
      var nurb = curve?.ToNurbsCurve();
      if (nurb == null)
        return false;

      var start_count = points.Count;
      var count = nurb.Points.Count * 8;
      var mul = 1.0 / count;
      var epsilon = nurb.Domain.Length > 1.0
        ? RhinoMath.ZeroTolerance 
        : nurb.Domain.Length * RhinoMath.ZeroTolerance;

      var t0 = 0.0;
      var t1 = 0.0;
      var kk0 = 0.0;
      var kk1 = 0.0;
      for (var i = 0; i <= count; i++)
      {
        var t2 = nurb.Domain.ParameterAt(i * mul);
        var k = nurb.CurvatureAt(t2);
        if (k.IsValid)
        {
          var kk2 = k * k;
          var bLeft = kk0 < kk1 - RhinoMath.ZeroTolerance;
          var bRight = kk2 < kk1 - RhinoMath.ZeroTolerance;

          if (bLeft && bRight)
          {
            if (FindMinRadius(nurb, t0, t2, kk1, epsilon, out var pt))
              points.Add(pt);
          }
          else if (bLeft && kk2 < kk1 + RhinoMath.ZeroTolerance)
          {
            var t = nurb.Domain.ParameterAt((t1 + t2) * 0.5);
            k = nurb.CurvatureAt(t);
            if (k.IsValid)
            {
              double kk = k * k;
              if (kk1 < kk - RhinoMath.ZeroTolerance)
              {
                if (FindMinRadius(nurb, t1, t2, kk, epsilon, out var pt))
                  points.Add(pt);
              }
            }
          }
          else if (bRight && kk0 < kk1 + RhinoMath.ZeroTolerance)
          {
            var t = nurb.Domain.ParameterAt((t0 + t1) * 0.5);
            k = nurb.CurvatureAt(t);
            if (k.IsValid)
            {
              var kk = k * k;
              if (kk1 < kk - RhinoMath.ZeroTolerance)
              {
                if (FindMinRadius(nurb, t0, t1, kk, epsilon, out var pt))
                  points.Add(pt);
              }
            }
          }

          t0 = t1;
          kk0 = kk1;

          t1 = t2;
          kk1 = kk2;
        }
      }

      return points.Count - start_count > 0;
    }

    private static bool FindMinRadius(NurbsCurve nurb, double t0, double t1, double kk, double epsilon, out Point3d pt)
    {
      pt = Point3d.Unset;

      var done = false;

      if (null == nurb)
        return false;

      for (var i = 0; i < 1000; i++)
      {
        Vector3d k;
        if (done || t1 - t0 < epsilon)
        {
          var t = (t0 + t1) * 0.5;
          k = nurb.CurvatureAt(t);
          pt = nurb.PointAt(t);
          return k.IsValid;
        }

        var tl = t0 * 0.75 + t1 * 0.25;
        k = nurb.CurvatureAt(tl);
        if (!k.IsValid)
          break;
        done = tl == t0;
        var kkl = k * k;

        var tr = t0 * 0.25 + t1 * 0.75;
        k = nurb.CurvatureAt(tr);
        if (!k.IsValid)
          break;
        done = tr == t1;
        var kkr = k * k;

        if (kkl > kkr && kkl > kk)
        {
          kk = kkl;
          t1 = (t0 + t1) * 0.5;
        }
        else if (kkr > kkl && kkr > kk)
        {
          kk = kkr;
          t0 = (t0 + t1) * 0.5;
        }
        else
        {
          t0 = tl;
          t1 = tr;
        }
      }

      return false;
    }
  }
}