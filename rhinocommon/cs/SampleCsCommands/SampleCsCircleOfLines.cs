using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsCircleOfLines : Command
  {
    public override string EnglishName => "SampleCsCircleOfLines";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var rc = Rhino.Input.RhinoGet.GetCircle(out var circle);
      if (rc != Result.Success)
        return rc;

      var n = 19;

      var points = PointsOnCircle(new Point3d(0, 0, 0), 5.0, n);
      for (var i = 0; i < n; i++)
      {
        for (var j = i + 1; j < n; j++)
        {
          var line = new Line(points[i], points[j]);
          doc.Objects.AddLine(line);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    Point3d[] PointsOnCircle(Point3d center, double radius, int n)
    {
      var alpha = Math.PI * 2 / n;
      var points = new Point3d[n];

      for (var i = 0; i < n; i++)
      {
        var theta = alpha * i;
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
