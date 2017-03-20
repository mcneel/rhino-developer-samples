using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("27efa9fd-c783-4f52-8afc-aebf1d8b0ae2")]
  public class SampleCsFilletSrf : Command
  {
    public SampleCsFilletSrf()
    {
      Radius = 1.0;
    }

    public override string EnglishName
    {
      get { return "SampleCsFilletSrf"; }
    }

    private double Radius { get; set; }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      double tolerance = doc.ModelAbsoluteTolerance;
      var opt_double = new OptionDouble(Radius, true, tolerance);

      // Select first surface to fillet
      var go0 = new GetObject();
      go0.SetCommandPrompt("Select first surface to fillet");
      go0.AddOptionDouble("Radius", ref opt_double, "Fillet radius");
      go0.GeometryFilter = ObjectType.Surface;
      go0.EnablePreSelect(false, true);
      for (;;)
      {
        GetResult res = go0.Get();
        if (res == GetResult.Option)
          continue;
        else if (res != GetResult.Object)
          return Result.Cancel;
        break;
      }

      var obj_ref0 = go0.Object(0);
      BrepFace face0 = obj_ref0.Face();
      if (null == face0)
        return Result.Failure;

      double u0, v0;
      if (null == obj_ref0.SurfaceParameter(out u0, out v0))
      {
        var pt = obj_ref0.SelectionPoint();
        if (!pt.IsValid || !face0.ClosestPoint(pt, out u0, out v0))
        {
          // Make surface selection scriptable
          u0 = face0.Domain(0).Min;
          v0 = face0.Domain(1).Min;
        }
      }

      // Juggle the pickpoint ever so slightly towards the middle of the domain
      // to get a better chance of getting a valid chord.
      if (u0 == face0.Domain(0).Min || u0 == face0.Domain(0).Max)
        u0 = 0.99 * u0 + 0.01 * face0.Domain(0).Mid;
      if (v0 == face0.Domain(1).Min || v0 == face0.Domain(1).Max)
        v0 = 0.99 * v0 + 0.01 * face0.Domain(1).Mid;

      // Select second surface to fillet
      var go1 = new GetObject();
      go1.SetCommandPrompt("Select second surface to fillet");
      go1.AddOptionDouble("Radius", ref opt_double, "Fillet radius");
      go1.GeometryFilter = ObjectType.Surface;
      go1.EnablePreSelect(false, true);
      go1.DeselectAllBeforePostSelect = false;
      for (;;)
      {
        GetResult res = go1.Get();
        if (res == GetResult.Option)
          continue;
        else if (res != GetResult.Object)
          return Result.Cancel;
        break;
      }

      var obj_ref1 = go1.Object(0);
      BrepFace face1 = obj_ref1.Face();
      if (null == face1)
        return Result.Failure;

      double u1, v1;
      if (null == obj_ref1.SurfaceParameter(out u1, out v1))
      {
        var pt = obj_ref1.SelectionPoint();
        if (!pt.IsValid || !face1.ClosestPoint(pt, out u1, out v1))
        {
          // Make surface selection scriptable
          u1 = face1.Domain(0).Min;
          v1 = face1.Domain(1).Min;
        }
      }

      // Juggle the pickpoint ever so slightly towards the middle of the domain
      // to get a better chance of getting a valid chord.
      if (u1 == face1.Domain(0).Min || u1 == face1.Domain(0).Max)
        u1 = 0.99 * u0 + 0.01 * face1.Domain(0).Mid;
      if (v1 == face1.Domain(1).Min || v1 == face1.Domain(1).Max)
        v1 = 0.99 * v0 + 0.01 * face1.Domain(1).Mid;

      var p0 = new Point2d(u0, v0);
      var p1 = new Point2d(u1, v1);
      var fillets = Surface.CreateRollingBallFillet(face0, p0, face1, p1, Radius, tolerance);

      foreach (var f in fillets)
        doc.Objects.AddSurface(f);

      doc.Views.Redraw();

      Radius = opt_double.CurrentValue;

      return Result.Success;
    }
  }
}
