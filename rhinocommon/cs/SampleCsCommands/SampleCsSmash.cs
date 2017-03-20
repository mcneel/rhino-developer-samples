using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("397f98ec-e57c-4649-bee4-afe99aa61b68")]
  public class SampleCsSmash : Command
  {
    public SampleCsSmash()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsSmash"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
      go.SetCommandPrompt("Select surface or polysurface to smash");
      go.GeometryFilter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter;
      go.Get();
      if (go.CommandResult() != Rhino.Commands.Result.Success)
        return go.CommandResult();

      Rhino.Geometry.Brep brep = go.Object(0).Brep();
      if (null == brep)
        return Rhino.Commands.Result.Failure;

      Rhino.Geometry.Unroller unroller = new Rhino.Geometry.Unroller(brep);
      unroller.AbsoluteTolerance = doc.ModelAbsoluteTolerance;
      unroller.RelativeTolerance = 1.0; // big relative tolerance for smash
      unroller.ExplodeSpacing = 2.0;
      unroller.ExplodeOutput = true;

      Rhino.Geometry.Curve[] unrolledCurves = null;
      Rhino.Geometry.Point3d[] unrolledPoints = null;
      Rhino.Geometry.TextDot[] unrolledDots = null;

      Rhino.Geometry.Brep[] output = unroller.PerformUnroll(out unrolledCurves, out unrolledPoints, out unrolledDots);
      if (null != output)
      {
        for (int i = 0; i < output.Length; i++)
          doc.Objects.Add(output[i]);
      }

      doc.Views.Redraw();

      return Rhino.Commands.Result.Success;
    }
  }
}
