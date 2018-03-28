using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsAddNurbsCurve : Command
  {
    public SampleCsAddNurbsCurve()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsAddNurbsCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const int degree = 3;
      const int cv_count = 6;
      const int knot_count = cv_count + degree - 1;
      const int order = degree + 1;

      var cvs = new Point3d[cv_count];
      cvs[0] = new Point3d(0.0, 0.0, 0.0);
      cvs[1] = new Point3d(5.0, 10.0, 0.0);
      cvs[2] = new Point3d(10.0, 0.0, 0.0);
      cvs[3] = new Point3d(15.0, 10.0, 0.0);
      cvs[4] = new Point3d(20.0, 0.0, 0.0);
      cvs[5] = new Point3d(25.0, 10.0, 0.0);

      var knots = new double[knot_count];
      knots[0] = 0.0;
      knots[1] = 0.0;
      knots[2] = 0.0;
      knots[3] = 1.0;
      knots[4] = 2.0;
      knots[5] = 3.0;
      knots[6] = 3.0;
      knots[7] = 3.0;

      var curve = new NurbsCurve(3, false, order, cv_count);

      for (int i = 0; i < cv_count; i++)
        curve.Points.SetPoint(i, cvs[i]);

      for (int i = 0; i < knot_count; i++)
        curve.Knots[i] = knots[i];

      if (curve.IsValid)
      {
        doc.Objects.AddCurve(curve);
        doc.Views.Redraw();
      } 
      
      return Result.Success;
    }
  }
}
