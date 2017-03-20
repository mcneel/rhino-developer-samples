using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("6aa65b22-36b9-4e6a-b20f-f140e316fda3")]
  public class SampleCsCircleOfLines : Command
  {
    public SampleCsCircleOfLines()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsCircleOfLines"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Geometry.Circle circle;
      Result rc = Rhino.Input.RhinoGet.GetCircle(out circle);
      if (rc != Result.Success)
        return rc;

      int n = 19;

      Rhino.Geometry.Point3d[] points = PointsOnCircle(new Rhino.Geometry.Point3d(0, 0, 0), 5.0, n);
      for (int i = 0; i < n; i++)
      {
        for (int j = i + 1; j < n; j++)
        {
          Rhino.Geometry.Line line = new Rhino.Geometry.Line(points[i], points[j]);
          doc.Objects.AddLine(line);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    Rhino.Geometry.Point3d[] PointsOnCircle(Rhino.Geometry.Point3d center, double radius, int n)
    {
      double alpha = Math.PI * 2 / n;
      Rhino.Geometry.Point3d[] points = new Rhino.Geometry.Point3d[n];

      for (int i = 0; i < n; i++)
      {
        double theta = alpha * i;
        points[i] = new Rhino.Geometry.Point3d(
            center.X + Math.Cos(theta) * radius,
            center.Y + Math.Sin(theta) * radius,
            center.Z
          );
      }
      return points;
    }
  }
}
