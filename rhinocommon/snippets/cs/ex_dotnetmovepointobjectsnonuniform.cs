using Rhino;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Input;

namespace examples_cs
{
  public class MovePointsNonUniformCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csMovePointsNonUniform"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef[] obj_refs;
      var rc = RhinoGet.GetMultipleObjects("Select points to move", false, ObjectType.Point, out obj_refs);
      if (rc != Result.Success || obj_refs == null)
        return rc;

      foreach (var obj_ref in obj_refs)
      {
        var point3d = obj_ref.Point().Location;
        // modify the point coordinates in some way ...
        point3d.X++;
        point3d.Y++;
        point3d.Z++;

        doc.Objects.Replace(obj_ref, point3d);
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}