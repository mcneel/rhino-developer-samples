using Rhino;
using Rhino.Collections;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;
using System.Linq;

namespace SampleCsCommands
{
  public class SampleCsPullGripsToMesh : Command
  {
    public override string EnglishName => "SampleCsPullGripsToMesh";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject get_grips = new GetObject();
      get_grips.SetCommandPrompt("Select grips to pull to mesh");
      get_grips.GeometryFilter = ObjectType.Grip;
      get_grips.GetMultiple(1, 0);
      if (get_grips.CommandResult() != Result.Success)
        return get_grips.CommandResult();

      TransformObjectList object_list = new TransformObjectList();
      object_list.AddObjects(get_grips, true);

      int grip_count = object_list.GripCount;
      if (0 == grip_count)
        return Result.Failure;

      GetObject get_mesh = new GetObject();
      get_mesh.SetCommandPrompt("Select mesh that pulls");
      get_mesh.GeometryFilter = ObjectType.Mesh;
      get_mesh.EnablePreSelect(false, true);
      get_mesh.DeselectAllBeforePostSelect = false;
      get_mesh.Get();
      if (get_mesh.CommandResult() != Result.Success)
        return get_mesh.CommandResult();

      Mesh mesh = get_mesh.Object(0).Mesh();
      if (null == mesh)
        return Result.Failure;

      GripObject[] grips = object_list.GripArray();
      Point3d[] locations = grips.Select(grip => grip.CurrentLocation).ToArray();
      if (locations.Length != grip_count)
        return Result.Failure;

      Point3d[] points = mesh.PullPointsToMesh(locations);
      if (null == points || points.Length != grip_count)
        return Result.Failure;

      for (int i = 0; i < grip_count; i++)
      {
        Vector3d dir = points[i] - grips[i].CurrentLocation;
        Transform xform = Transform.Translation(dir);
        if (xform.IsValid)
          grips[i].Move(xform);
      }

      foreach (RhinoObject owner in object_list.GripOwnerArray())
        doc.Objects.GripUpdate(owner, true);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}