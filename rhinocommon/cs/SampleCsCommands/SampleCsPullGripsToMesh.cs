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
      var get_grips = new GetObject();
      get_grips.SetCommandPrompt("Select grips to pull to mesh");
      get_grips.GeometryFilter = ObjectType.Grip;
      get_grips.GetMultiple(1, 0);
      if (get_grips.CommandResult() != Result.Success)
        return get_grips.CommandResult();

      var object_list = new TransformObjectList();
      object_list.AddObjects(get_grips, true);

      var grip_count = object_list.GripCount;
      if (0 == grip_count)
        return Result.Failure;

      var get_mesh = new GetObject();
      get_mesh.SetCommandPrompt("Select mesh that pulls");
      get_mesh.GeometryFilter = ObjectType.Mesh;
      get_mesh.EnablePreSelect(false, true);
      get_mesh.DeselectAllBeforePostSelect = false;
      get_mesh.Get();
      if (get_mesh.CommandResult() != Result.Success)
        return get_mesh.CommandResult();

      var mesh = get_mesh.Object(0).Mesh();
      if (null == mesh)
        return Result.Failure;

      var grips = object_list.GripArray();
      var locations = grips.Select(grip => grip.CurrentLocation).ToArray();
      if (locations.Length != grip_count)
        return Result.Failure;

      var points = mesh.PullPointsToMesh(locations);
      if (null == points || points.Length != grip_count)
        return Result.Failure;

      for (var i = 0; i < grip_count; i++)
      {
        var dir = points[i] - grips[i].CurrentLocation;
        var xform = Transform.Translation(dir);
        if (xform.IsValid)
          grips[i].Move(xform);
      }

      foreach (var owner in object_list.GripOwnerArray())
        doc.Objects.GripUpdate(owner, true);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}