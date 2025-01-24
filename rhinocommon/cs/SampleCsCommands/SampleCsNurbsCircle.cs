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
      // The degree must be >= 1 and the number of control points
      const int degree = 2;
      // The order is degree + 1
      const int order = degree + 1;
      // The number of control points
      const int cv_count = 7;
      // The number of knots is always (number of control points + degree - 1)
      const int knot_count = cv_count + degree - 1;

      // Define the "Euclidean" (world 3-D) locations for the control points.
      Point3d[] points = new Point3d[cv_count];
      points[0] = new Point3d(2.500, 0.000, 0.000);
      points[1] = new Point3d(5.000, 0.000, 0.000);
      points[2] = new Point3d(3.750, 2.165, 0.000);
      points[3] = new Point3d(2.500, 4.330, 0.000);
      points[4] = new Point3d(1.250, 2.165, 0.000);
      points[5] = new Point3d(0.000, 0.000, 0.000);
      points[6] = new Point3d(2.500, 0.000, 0.000);

      // Define the weights
      // Weights must be > 0.
      // In general you should set the first and last weight to 1.
      double[] weights = new double[cv_count];
      weights[0] = 1.0;
      weights[1] = 0.5;
      weights[2] = 1.0;
      weights[3] = 0.5;
      weights[4] = 1.0;
      weights[5] = 0.5;
      weights[6] = 1.0;

      // Define the knots.
      // Unless you are doing something special, knot vectors should be clamped.
      // "Clamped" means the first and last degree many knots are the same.
      // In this example the first three knots are 0 and the last three knots are 3.
      // The interior knots can have multiplicity from 1 (a "simple" knot)
      // to degree (a "full multiplicity")
      double[] knots = new double[knot_count];
      // Start with a full multiplicity knot
      knots[0] = 0.000;
      knots[1] = 0.000;
      // Full multiplicity interior knot
      knots[2] = 0.333;
      knots[3] = 0.333;
      // Full multiplicity interior knot
      knots[4] = 0.667;
      knots[5] = 0.667;
      // End with a full multiplicity knot
      knots[6] = 1.000;
      knots[7] = 1.000;

      // Create a rational NURBS curve
      NurbsCurve curve = new NurbsCurve(3, true, order, cv_count);

      // Set the control points and weights.
      // Since our curve is rational, we need homogeneous points (4-D
      for (int ci = 0; ci < cv_count; ci++)
      {
        Point4d cv = new Point4d(points[ci].X * weights[ci], points[ci].Y * weights[ci], points[ci].Z * weights[ci], weights[ci]);
        curve.Points.SetPoint(ci, cv);
      }

      // Set the knots
      for (int ki = 0; ki < knot_count; ki++)
        curve.Knots[ki] = knots[ki];

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
