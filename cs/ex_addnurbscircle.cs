using System;

partial class Examples
{
  public static Rhino.Commands.Result AddNurbsCircle(Rhino.RhinoDoc doc)
  {
    // The easy way to get a NURBS curve from a circle is with
    // the following two lines of code.
    //
    // Rhino.Geometry.Circle c = new Rhino.Geometry.Circle(20);
    // Rhino.Geometry.NurbsCurve nc = c.ToNurbsCurve();
    //
    // This sample demonstrates creating a NURBS curve from scratch.
    const int dimension = 3;
    const bool isRational = true;
    const int order = 3;
    const int cv_count = 9;
    Rhino.Geometry.NurbsCurve nc = new Rhino.Geometry.NurbsCurve(dimension, isRational, order, cv_count);
    nc.Points.SetPoint(0, 1.0, 0.0, 0.0, 1.0);
    nc.Points.SetPoint(1, 0.707107, 0.707107, 0.0, 0.707107);
    nc.Points.SetPoint(2, 0.0, 1.0, 0.0, 1.0);
    nc.Points.SetPoint(3, -0.707107, 0.707107, 0.0, 0.707107);
    nc.Points.SetPoint(4, -1.0, 0.0, 0.0, 1.0);
    nc.Points.SetPoint(5, -0.707107, -0.707107, 0.0, 0.707107);
    nc.Points.SetPoint(6, 0.0, -1.0, 0.0, 1.0);
    nc.Points.SetPoint(7, 0.707107, -0.707107, 0.0, 0.707107);
    nc.Points.SetPoint(8, 1.0, 0.0, 0.0, 1.0);
    nc.Knots[0] = 0.0;
    nc.Knots[1] = 0.0;
    nc.Knots[2] = 0.5 * Math.PI;
    nc.Knots[3] = 0.5 * Math.PI;
    nc.Knots[4] = Math.PI;
    nc.Knots[5] = Math.PI;
    nc.Knots[6] = 1.5 * Math.PI;
    nc.Knots[7] = 1.5 * Math.PI;
    nc.Knots[8] = 2.0 * Math.PI;
    nc.Knots[9] = 2.0 * Math.PI;
    if (nc.IsValid)
    {
      doc.Objects.AddCurve(nc);
      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }
    return Rhino.Commands.Result.Failure;
  }
}
