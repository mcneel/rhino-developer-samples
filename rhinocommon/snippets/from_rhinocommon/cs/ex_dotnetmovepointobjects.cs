using Rhino;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;
using Rhino.Geometry;

namespace examples_cs
{
  public class MovePointsCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csMovePoints"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef[] obj_refs;
      var rc = RhinoGet.GetMultipleObjects("Select points to move", false, ObjectType.Point, out obj_refs);
      if (rc != Result.Success || obj_refs == null)
        return rc;

      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to move from");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();
      var start_point = gp.Point();
  
      gp.SetCommandPrompt("Point to move to");
      gp.SetBasePoint(start_point, false);
      gp.DrawLineFromPoint(start_point, true);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();
      var end_point = gp.Point();

      var xform = Transform.Translation(end_point - start_point);

      foreach (var obj_ref in obj_refs)
      {
        doc.Objects.Transform(obj_ref, xform, true);
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}