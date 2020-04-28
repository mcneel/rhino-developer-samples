using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsNurbsCircle : Command
  {
    public override string EnglishName => "SampleCsNurbsCircle";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const int degree = 2;
      const int cv_count = 7;
      const int knot_count = cv_count + degree - 1;
      const int order = degree + 1;

      var points = new Point3d[cv_count];
      points[0] = new Point3d(2.500, 0.000, 0.000);
      points[1] = new Point3d(5.000, 0.000, 0.000);
      points[2] = new Point3d(3.750, 2.165, 0.000);
      points[3] = new Point3d(2.500, 4.330, 0.000);
      points[4] = new Point3d(1.250, 2.165, 0.000);
      points[5] = new Point3d(0.000, 0.000, 0.000);
      points[6] = new Point3d(2.500, 0.000, 0.000);
 
      var weights = new double[cv_count];
      weights[0] = 1.0;
      weights[1] = 0.5;
      weights[2] = 1.0;
      weights[3] = 0.5;
      weights[4] = 1.0;
      weights[5] = 0.5;
      weights[6] = 1.0;

      var knots = new double[knot_count];
      knots[0] = 0.000;
      knots[1] = 0.000;
      knots[2] = 0.333;
      knots[3] = 0.333;
      knots[4] = 0.667;
      knots[5] = 0.667;
      knots[6] = 1.000;
      knots[7] = 1.000;

      var curve = new NurbsCurve(3, true, order, cv_count);

      for (var ci = 0; ci < cv_count; ci++)
      {
        var cv = new Point4d(points[ci].X * weights[ci], points[ci].Y * weights[ci], points[ci].Z * weights[ci], weights[ci]);
        curve.Points.SetPoint(ci, cv);
      }

      for (var ki = 0; ki < knot_count; ki++)
        curve.Knots[ki] = knots[ki];

      if (curve.IsValid)
      {
        doc.Objects.AddCurve(curve);
        doc.Views.Redraw();
      } 

      return Result.Success;
    }
  }
}
