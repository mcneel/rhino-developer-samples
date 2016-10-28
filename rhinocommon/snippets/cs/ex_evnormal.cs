using Rhino;
using Rhino.Input.Custom;
using Rhino.DocObjects;
using Rhino.Commands;

namespace examples_cs
{
  public class NormalDirectionOfBrepFaceCommand : Command
  {
    public override string EnglishName { get { return "csDetermineNormalDirectionOfBrepFace"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // select a surface
      var gs = new GetObject();
      gs.SetCommandPrompt("select surface");
      gs.GeometryFilter = ObjectType.Surface;
      gs.DisablePreSelect();
      gs.SubObjectSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();
      // get the selected face
      var face = gs.Object(0).Face();
      if (face == null)
        return Result.Failure;

      // pick a point on the surface.  Constain
      // picking to the face.
      var gp = new GetPoint();
      gp.SetCommandPrompt("select point on surface");
      gp.Constrain(face, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      // get the parameters of the point on the
      // surface that is clesest to gp.Point()
      double u, v;
      if (face.ClosestPoint(gp.Point(), out u, out v))
      {
        var direction = face.NormalAt(u, v);
        if (face.OrientationIsReversed)
          direction.Reverse();
        RhinoApp.WriteLine(
          string.Format(
            "Surface normal at uv({0:f},{1:f}) = ({2:f},{3:f},{4:f})", 
            u, v, direction.X, direction.Y, direction.Z));
      }
      return Result.Success;
    }
  }
}