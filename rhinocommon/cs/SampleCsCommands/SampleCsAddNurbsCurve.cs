using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsAddNurbsCurve : Command
  {
    public override string EnglishName => "SampleCsAddNurbsCurve";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // The degree must be >= 1 and the number of control points
      const int degree = 3;
      // The order is degree + 1
      const int order = degree + 1;
      // The number of control points
      const int cv_count = 6;
      // The number of knots is always (number of control points + degree - 1)
      const int knot_count = cv_count + degree - 1;

      // Define the "Euclidean" (world 3-D) locations for the control points.
      Point3d[] cvs = new Point3d[cv_count];
      cvs[0] = new Point3d(0.0, 0.0, 0.0);
      cvs[1] = new Point3d(5.0, 10.0, 0.0);
      cvs[2] = new Point3d(10.0, 0.0, 0.0);
      cvs[3] = new Point3d(15.0, 10.0, 0.0);
      cvs[4] = new Point3d(20.0, 0.0, 0.0);
      cvs[5] = new Point3d(25.0, 10.0, 0.0);

      // Define the knots.
      // Unless you are doing something special, knot vectors should be clamped.
      // "Clamped" means the first and last degree many knots are the same.
      // In this example the first three knots are 0 and the last three knots are 3.
      // The interior knots can have multiplicity from 1 (a "simple" knot)
      // to degree (a "full multiplicity")
      double[] knots = new double[knot_count];
      // Start with a full multiplicity knot
      knots[0] = 0.0;
      knots[1] = 0.0;
      knots[2] = 0.0;
      // Simple interior knot
      knots[3] = 1.0;
      // Simple interior knot
      knots[4] = 2.0;
      // End with a full multiplicity knot
      knots[5] = 3.0;
      knots[6] = 3.0;
      knots[7] = 3.0;

      // Create a non-rational NURBS curve
      NurbsCurve curve = new NurbsCurve(3, false, order, cv_count);

      // Set the control points
      for (int i = 0; i < cv_count; i++)
        curve.Points.SetPoint(i, cvs[i]);

      // Set the knots
      for (int i = 0; i < knot_count; i++)
        curve.Knots[i] = knots[i];

      if (curve.IsValid)
      {
        // Parameterization should match the length of a curve
        double length = curve.GetLength();
        Interval domain = new Interval(0.0, length);
        curve.Domain = domain;

        doc.Objects.AddCurve(curve);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
