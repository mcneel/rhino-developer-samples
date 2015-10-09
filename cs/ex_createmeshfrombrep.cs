using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Geometry;
using Rhino.DocObjects;

namespace examples_cs
{
  public class CreateMeshFromBrepCommand : Command
  {
    public override string EnglishName { get { return "csCreateMeshFromBrep"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;
      var rc = RhinoGet.GetOneObject("Select surface or polysurface to mesh", true, ObjectType.Surface | ObjectType.PolysrfFilter, out obj_ref);
      if (rc != Result.Success)
        return rc;
      var brep = obj_ref.Brep();
      if (null == brep)
        return Result.Failure;

      // you could choose anyone of these for example
      var jagged_and_faster = MeshingParameters.Coarse;
      var smooth_and_slower = MeshingParameters.Smooth;
      var default_mesh_params = MeshingParameters.Default;
      var minimal = MeshingParameters.Minimal;

      var meshes = Mesh.CreateFromBrep(brep, smooth_and_slower);
      if (meshes == null || meshes.Length == 0)
        return Result.Failure;

      var brep_mesh = new Mesh();
      foreach (var mesh in meshes)
        brep_mesh.Append(mesh);
      doc.Objects.AddMesh(brep_mesh);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}