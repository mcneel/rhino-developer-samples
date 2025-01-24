using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsExtractInflectionPoints : Command
  {
    public override string EnglishName => "SampleCsExtractInflectionPoints";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select curves to extract inflection points");
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
          if (GetInflectionPoints(curve, points))
            doc.Objects.AddPoints(points);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    public static bool GetInflectionPoints(Curve curve, List<Point3d> points)
    {
      const double on_zero_tolerance = 2.3283064365386962890625e-10;

      NurbsCurve nurb = curve?.ToNurbsCurve();
      if (nurb == null)
        return false;

      int count = nurb.Points.Count * 4;
      double mul = 1.0 / count;
      double epsilon = nurb.Domain.Length > 1.0
        ? on_zero_tolerance
        : nurb.Domain.Length * on_zero_tolerance;

      double t0 = 0.0;
      Vector3d k0 = Vector3d.Unset;
      bool start_set = false;
      for (int i = 0; i <= count; i++)
      {
        double t1 = nurb.Domain.ParameterAt(i * mul);
        Vector3d k1 = nurb.CurvatureAt(t1);
        if (k1.IsValid)
        {
          if (k1.IsTiny())
            continue;

          if (!start_set)
          {
            t0 = t1;
            k0 = k1;
            start_set = true;
            continue;
          }

          if (k0 * k1 < 0.0)
          {
            if (FindInflection(nurb, t0, t1, k0, k1, epsilon, out Point3d pt))
              points.Add(pt);
          }

          k0 = k1;
          t0 = t1;
        }
      }

      return points.Count > 0;
    }

    private static bool FindInflection(NurbsCurve nurb, double t0, double t1, Vector3d k0, Vector3d k1, double epsilon, out Point3d pt)
    {
      pt = Point3d.Unset;

      if (null == nurb)
        return false;

      bool rc = false;
      for (; ; )
      {
        double t = (t0 + t1) * 0.5;
        Vector3d k = nurb.CurvatureAt(t);
        if (!k.IsValid)
          break;

        if (k.IsTiny() || t1 - t0 < epsilon)
        {
          pt = nurb.PointAt(t);
          rc = true;
          break;
        }

        if (k * k0 < 0.0)
        {
          t1 = t;
          k1 = k;
          continue;
        }

        if (k * k1 < 0.0)
        {
          t0 = t;
          k0 = k;
          continue;
        }

        break;
      }

      return rc;
    }
  }
}