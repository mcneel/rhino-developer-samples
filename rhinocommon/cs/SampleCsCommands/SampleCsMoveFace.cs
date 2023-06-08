using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsMoveFace : Command
  {
    public override string EnglishName => "SampleCsMoveFace";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select face to move");
      go.GeometryFilter = ObjectType.Surface;
      go.GeometryAttributeFilter = GeometryAttributeFilter.SubSurface;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var objref = go.Object(0);
      var brep = objref.Brep();
      var face = objref.Face();
      if (null == brep || null == face)
        return Result.Failure;

      var domU = face.Domain(0);
      var domV = face.Domain(1);
      var origin = face.PointAt(domU.Mid, domV.Mid);
      var normal = face.NormalAt(domU.Mid, domV.Mid);
      normal.Unitize();

      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to move face to");
      gp.Constrain(origin, origin + normal);
      gp.SetBasePoint(origin, true);
      gp.DrawLineFromPoint(origin, true);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var offset = gp.Point();
      var dir = offset - origin;
      if (dir.IsTiny())
        return Result.Nothing;

      var xform = Transform.Translation(dir);
      var bdupe = brep.DuplicateBrep();
      var ci = new ComponentIndex[] { face.ComponentIndex() };
      var rc = bdupe.TransformComponent(ci, xform, doc.ModelAbsoluteTolerance, 0, false);
      if (rc)
      {
        doc.Objects.Replace(objref.ObjectId, bdupe);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}