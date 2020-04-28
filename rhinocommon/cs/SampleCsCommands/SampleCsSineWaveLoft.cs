using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsSineWaveLoft : Command
  {
    public override string EnglishName => "SampleCsSineWaveLoft";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const int u_count = 25;
      const int v_count = 25;

      var curves = new List<Curve>();
      var points = new List<Point3d>();

      for (var u = 0; u <= u_count; u++)
      {
        points.Clear();
        for (var v = 0; v <= v_count; v++)
          points.Add(new Point3d(u, v, Math.Sin(u) + Math.Sin(v)));
        curves.Add(Curve.CreateInterpolatedCurve(points, 3));
      }

      var breps = Brep.CreateFromLoft(curves, Point3d.Unset, Point3d.Unset, LoftType.Normal, false);
      if (null != breps)
      {
        foreach (var brep in breps)
          doc.Objects.AddBrep(brep);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
