using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsMeshBrep : Command
  {
    public override string EnglishName => "SampleCsMeshBrep";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjRef obj_ref;

      const string prompt = "Select surface or polysurface to mesh";
      const Rhino.DocObjects.ObjectType object_type =
        Rhino.DocObjects.ObjectType.Surface |
        Rhino.DocObjects.ObjectType.Brep;

      Result res = Rhino.Input.RhinoGet.GetOneObject(prompt, false, object_type, out obj_ref);
      if (res != Result.Success)
        return res;

      Rhino.DocObjects.RhinoObject obj = obj_ref.Object();
      if (null == obj)
        return Result.Failure;

      Rhino.Geometry.Mesh obj_mesh = new Rhino.Geometry.Mesh();

      Rhino.Geometry.Mesh[] meshes = obj.GetMeshes(Rhino.Geometry.MeshType.Render);
      if (meshes.Length > 0)
      {
        foreach (Rhino.Geometry.Mesh m in meshes)
          obj_mesh.Append(m);
      }
      else
      {
        Rhino.Geometry.MeshingParameters mesh_params = obj.GetRenderMeshParameters();
        if (obj.CreateMeshes(Rhino.Geometry.MeshType.Render, mesh_params, false) > 0)
        {
          meshes = obj.GetMeshes(Rhino.Geometry.MeshType.Render);
          if (meshes.Length > 0)
          {
            foreach (Rhino.Geometry.Mesh m in meshes)
              obj_mesh.Append(m);
          }
        }
      }

      if (obj_mesh.IsValid)
        obj_mesh.Faces.ConvertQuadsToTriangles();

      if (obj_mesh.IsValid)
      {
        doc.Objects.AddMesh(obj_mesh);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
