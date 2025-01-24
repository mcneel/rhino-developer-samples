using Rhino;
using Rhino.Collections;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsMoveGrips : Command
  {
    public override string EnglishName => "SampleCsMoveGrips";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select grips to move");
      go.GeometryFilter = ObjectType.Grip;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      using (TransformObjectList object_list = new TransformObjectList())
      {
        object_list.AddObjects(go, true);

        Vector3d dir = new Vector3d(5, 0, 0); // 5 units in world x-axis direction
        Transform xform = Transform.Translation(dir);

        foreach (GripObject grip in object_list.GripArray())
          grip.Move(xform);

        foreach (RhinoObject owner in object_list.GripOwnerArray())
          doc.Objects.GripUpdate(owner, true);

        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}