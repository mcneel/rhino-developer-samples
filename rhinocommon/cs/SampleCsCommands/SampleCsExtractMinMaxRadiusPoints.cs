using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsExtractMinMaxRadiusPoints : Command
  {
    public override string EnglishName => "SampleCsExtractMinMaxRadiusPoints";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select curves to extract min/max radius points");
      go.GeometryFilter = ObjectType.Curve;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (int i = 0; i < go.ObjectCount; i++)
      {
        Curve curve = go.Object(i).Curve();
        if (null != curve)
        {
          List<Point3d> points = new List<Point3d>();
          if (GetMinMaxRadiusPoints(curve, points))
            doc.Objects.AddPoints(points);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    public static bool GetMinMaxRadiusPoints(Curve curve, List<Point3d> points)
    {
      NurbsCurve nurb = curve?.ToNurbsCurve();
      if (nurb == null)
        return false;

      int start_count = points.Count;
      int count = nurb.Points.Count * 8;
      double mul = 1.0 / count;
      double epsilon = nurb.Domain.Length > 1.0
        ? RhinoMath.ZeroTolerance
        : nurb.Domain.Length * RhinoMath.ZeroTolerance;

      double t0 = 0.0;
      double t1 = 0.0;
      double kk0 = 0.0;
      double kk1 = 0.0;
      for (int i = 0; i <= count; i++)
      {
        double t2 = nurb.Domain.ParameterAt(i * mul);
        Vector3d k = nurb.CurvatureAt(t2);
        if (k.IsValid)
        {
          double kk2 = k * k;
          bool bLeft = kk0 < kk1 - RhinoMath.ZeroTolerance;
          bool bRight = kk2 < kk1 - RhinoMath.ZeroTolerance;

          if (bLeft && bRight)
          {
            if (FindMinRadius(nurb, t0, t2, kk1, epsilon, out Point3d pt))
              points.Add(pt);
          }
          else if (bLeft && kk2 < kk1 + RhinoMath.ZeroTolerance)
          {
            double t = nurb.Domain.ParameterAt((t1 + t2) * 0.5);
            k = nurb.CurvatureAt(t);
            if (k.IsValid)
            {
              double kk = k * k;
              if (kk1 < kk - RhinoMath.ZeroTolerance)
              {
                if (FindMinRadius(nurb, t1, t2, kk, epsilon, out Point3d pt))
                  points.Add(pt);
              }
            }
          }
          else if (bRight && kk0 < kk1 + RhinoMath.ZeroTolerance)
          {
            double t = nurb.Domain.ParameterAt((t0 + t1) * 0.5);
            k = nurb.CurvatureAt(t);
            if (k.IsValid)
            {
              double kk = k * k;
              if (kk1 < kk - RhinoMath.ZeroTolerance)
              {
                if (FindMinRadius(nurb, t0, t1, kk, epsilon, out Point3d pt))
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

      bool done = false;

      if (null == nurb)
        return false;

      for (int i = 0; i < 1000; i++)
      {
        Vector3d k;
        if (done || t1 - t0 < epsilon)
        {
          double t = (t0 + t1) * 0.5;
          k = nurb.CurvatureAt(t);
          pt = nurb.PointAt(t);
          return k.IsValid;
        }

        double tl = t0 * 0.75 + t1 * 0.25;
        k = nurb.CurvatureAt(tl);
        if (!k.IsValid)
          break;
        done = tl == t0;
        double kkl = k * k;

        double tr = t0 * 0.25 + t1 * 0.75;
        k = nurb.CurvatureAt(tr);
        if (!k.IsValid)
          break;
        done = tr == t1;
        double kkr = k * k;

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