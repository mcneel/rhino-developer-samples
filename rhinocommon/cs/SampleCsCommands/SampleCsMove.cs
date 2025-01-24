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
      Result rc = RhinoGet.GetOneObject("Select object to move", false, ObjectType.AnyObject, out ObjRef objref);
      if (rc != Result.Success)
        return rc;
      if (null == objref)
        return Result.Failure;

      rc = RhinoGet.GetPoint("Point to move from", false, out Point3d first_point);
      if (rc != Result.Success)
        return rc;

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Point to move to");
      gp.SetBasePoint(first_point, true);
      gp.DrawLineFromPoint(first_point, true);
      gp.Get();
      rc = gp.CommandResult();
      if (rc != Result.Success)
        return rc;

      Point3d second_point = gp.Point();

      Vector3d dir = second_point - first_point;
      if (dir.Length > RhinoMath.ZeroTolerance)
      {
        Transform xform = Transform.Translation(dir);
        doc.Objects.Transform(objref, xform, true);
        doc.Views.Redraw();
      }

      return rc;
    }
  }
}
