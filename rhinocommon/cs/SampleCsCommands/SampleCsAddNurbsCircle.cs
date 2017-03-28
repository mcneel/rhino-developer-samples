using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("e021ecb0-41ed-455b-a3de-014345c17923")]
  public class SampleCsAddNurbsCircle : Command
  {
    public SampleCsAddNurbsCircle()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsAddNurbsCircle"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const int order = 3; // order = degree + 1
      const int cv_count = 9;
 
      var curve = new NurbsCurve(3, true, order, cv_count);

      curve.Points.SetPoint(0, 1.0, 0.0, 0.0, 1.0);
      curve.Points.SetPoint(1, 0.707107, 0.707107, 0.0, 0.707107);
      curve.Points.SetPoint(2, 0.0, 1.0, 0.0, 1.0);
      curve.Points.SetPoint(3, -0.707107, 0.707107, 0.0, 0.707107);
      curve.Points.SetPoint(4, -1.0, 0.0, 0.0, 1.0);
      curve.Points.SetPoint(5, -0.707107, -0.707107, 0.0, 0.707107);
      curve.Points.SetPoint(6, 0.0, -1.0, 0.0, 1.0);
      curve.Points.SetPoint(7, 0.707107, -0.707107, 0.0, 0.707107);
      curve.Points.SetPoint(8, 1.0, 0.0, 0.0, 1.0);

      curve.Knots[0] = 0.0;
      curve.Knots[1] = 0.0;
      curve.Knots[2] = 0.5 * Math.PI;
      curve.Knots[3] = 0.5 * Math.PI;
      curve.Knots[4] = Math.PI;
      curve.Knots[5] = Math.PI;
      curve.Knots[6] = 1.5 * Math.PI;
      curve.Knots[7] = 1.5 * Math.PI;
      curve.Knots[8] = 2.0 * Math.PI;
      curve.Knots[9] = 2.0 * Math.PI;
 
      if (curve.IsValid)
      {
        doc.Objects.AddCurve(curve);
        doc.Views.Redraw();
      } 
      
      return Result.Success;

    }
  }
}
