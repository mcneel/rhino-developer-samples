using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsIntersectCurveBrepFace : Command
  {
    public override string EnglishName => "SampleCsIntersectCurveBrepFace";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gs = new GetObject();
      gs.SetCommandPrompt("Select surface");
      gs.GeometryFilter = Rhino.DocObjects.ObjectType.Surface;
      gs.SubObjectSelect = true;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var face = gs.Object(0).Face();
      if (null == face)
        return Result.Failure;

      var gc = new GetObject();
      gc.SetCommandPrompt("Select curve");
      gc.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
      gc.EnablePreSelect(false, true);
      gc.DeselectAllBeforePostSelect = false;
      gc.Get();
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      var curve = gc.Object(0).Curve();
      if (null == curve)
        return Result.Failure;

      var tol = doc.ModelAbsoluteTolerance;
      var rc = Intersection.CurveBrepFace(curve, face, tol, out Curve[] outCurves, out Point3d[] outPoints);
      if (rc)
      {
        foreach (var c in outCurves)
          doc.Objects.AddCurve(c);

        foreach (var pt in outPoints)
          doc.Objects.AddPoint(pt);

        doc.Views.Redraw();
      }
      return Result.Success;
    }
  }
}