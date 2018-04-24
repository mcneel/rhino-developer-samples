using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsUnweldAll : Command
  {
    public override string EnglishName => "SampleCsUnweldAll";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select polygon meshes to unweld");
      go.GeometryFilter = ObjectType.Mesh;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var obj_ref in go.Objects())
      {
        var old_mesh = obj_ref.Mesh();
        if (null != old_mesh)
        {
          var new_mesh = UnweldMesh(old_mesh);
          if (null != new_mesh && new_mesh.IsValid)
            doc.Objects.Replace(go.Object(0), new_mesh);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    private static Mesh UnweldMesh(Mesh src)
    {
      if (null == src) throw new ArgumentNullException(nameof(src));

      var mesh = src.DuplicateMesh();
      for (var i = 0; i < mesh.Faces.Count; i++)
      {
        var face = mesh.Faces[i];
        var a = mesh.Vertices.Add(mesh.Vertices[face.A]);
        var b = mesh.Vertices.Add(mesh.Vertices[face.B]);
        var c = mesh.Vertices.Add(mesh.Vertices[face.C]);
        var d = face.IsQuad ? mesh.Vertices.Add(mesh.Vertices[face.D]) : c;
        mesh.Faces.SetFace(i, a, b, c, d);
      }

      mesh.Compact();
      mesh.Normals.ComputeNormals();
      mesh.FaceNormals.ComputeFaceNormals();

      return mesh;
    }
  }
}