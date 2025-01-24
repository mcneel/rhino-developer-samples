using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsCreateUVCurve : Command
  {
    public override string EnglishName => "SampleCsCreateUVCurve";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select surface to create UV curve");
      go.GeometryFilter = ObjectType.Surface;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      Rhino.Geometry.BrepFace brep_face = go.Object(0).Face();
      if (null == brep_face)
        return Result.Failure;

      foreach (Rhino.Geometry.BrepLoop loop in brep_face.Loops)
      {
        Rhino.Geometry.Curve loop_curve = loop.To2dCurve();
        loop_curve.ChangeDimension(3);
        doc.Objects.AddCurve(loop_curve);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}