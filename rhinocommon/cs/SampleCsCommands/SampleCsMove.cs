using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsMove : Command
  {
    public override string EnglishName => "SampleCsMove";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef objref;
      var rc = RhinoGet.GetOneObject("Select object to move", false, ObjectType.AnyObject, out objref);
      if (rc != Result.Success)
        return rc;
      if (null == objref)
        return Result.Failure;

      Point3d first_point;
      rc = RhinoGet.GetPoint("Point to move from", false, out first_point);
      if (rc != Result.Success)
        return rc;

      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to move to");
      gp.SetBasePoint(first_point, true);
      gp.DrawLineFromPoint(first_point, true);
      gp.Get();
      rc = gp.CommandResult();
      if (rc != Result.Success)
        return rc;

      var second_point = gp.Point();

      var dir = second_point - first_point;
      if (dir.Length > RhinoMath.ZeroTolerance)
      {
        var xform = Transform.Translation(dir);
        doc.Objects.Transform(objref, xform, true);
        doc.Views.Redraw();
      }

      return rc;
    }
  }
}
