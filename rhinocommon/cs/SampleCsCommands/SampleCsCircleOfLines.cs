using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using System;

namespace SampleCsCommands
{
  public class SampleCsCircleOfLines : Command
  {
    public override string EnglishName => "SampleCsCircleOfLines";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Result rc = Rhino.Input.RhinoGet.GetCircle(out Circle circle);
      if (rc != Result.Success)
        return rc;

      int n = 19;

      Point3d[] points = PointsOnCircle(new Point3d(0, 0, 0), 5.0, n);
      for (int i = 0; i < n; i++)
      {
        for (int j = i + 1; j < n; j++)
        {
          Line line = new Line(points[i], points[j]);
          doc.Objects.AddLine(line);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    Point3d[] PointsOnCircle(Point3d center, double radius, int n)
    {
      double alpha = Math.PI * 2 / n;
      Point3d[] points = new Point3d[n];

      for (int i = 0; i < n; i++)
      {
        double theta = alpha * i;
        points[i] = new Point3d(
            center.X + Math.Cos(theta) * radius,
            center.Y + Math.Sin(theta) * radius,
            center.Z
          );
      }
      return points;
    }
  }
}
