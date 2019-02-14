using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsWriteStl : Command
  {
    public override string EnglishName => "SampleCsWriteStl";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Mesh mesh = new Mesh();

      mesh.Vertices.Add(new Rhino.Geometry.Point3d(0.5, 0.5, 0.5));
      mesh.Vertices.Add(new Rhino.Geometry.Point3d(0.5, 0.5, -0.5));
      mesh.Vertices.Add(new Rhino.Geometry.Point3d(0.5, -0.5, 0.5));
      mesh.Vertices.Add(new Rhino.Geometry.Point3d(0.5, -0.5, -0.5));
      mesh.Vertices.Add(new Rhino.Geometry.Point3d(-0.5, 0.5, 0.5));
      mesh.Vertices.Add(new Rhino.Geometry.Point3d(-0.5, 0.5, -0.5));
      mesh.Vertices.Add(new Rhino.Geometry.Point3d(-0.5, -0.5, 0.5));
      mesh.Vertices.Add(new Rhino.Geometry.Point3d(-0.5, -0.5, -0.5));

      mesh.Faces.AddFace(0, 1, 5, 4);
      mesh.Faces.AddFace(0, 4, 6, 2);
      mesh.Faces.AddFace(0, 2, 3, 1);
      mesh.Faces.AddFace(7, 3, 2, 6);
      mesh.Faces.AddFace(7, 6, 4, 5);
      mesh.Faces.AddFace(7, 5, 1, 3);

      mesh.FaceNormals.ComputeFaceNormals();
      mesh.Normals.ComputeNormals();
      mesh.Compact();

      var folder = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
      var path = Path.Combine(folder, "SampleCsWriteStl.stl");
      FileStl.WriteFile(mesh, path);

      return Result.Success;
    }
  }

  /// <summary>
  /// Simple class to write STL files.
  /// </summary>
  public static class FileStl
  {
    /// <summary>
    /// Write a Rhino.Geometry.Mesh object to an ASCII file in STL format.
    /// </summary>
    public static bool WriteFile(Mesh mesh, string path)
    {
      return WriteFile(new Mesh[] { mesh }, path);
    }

    /// <summary>
    /// Write a list of Rhino.Geometry.Mesh object to an ASCII file in STL format.
    /// </summary>
    public static bool WriteFile(IList<Mesh> meshes, string path)
    {
      try
      {
        var model = WriteString(meshes);
        File.WriteAllText(path, model);
      }
      catch (Exception)
      {
        return false;
      }
      return true;
    }

    /// <summary>
    /// Write a Rhino.Geometry.Mesh object to an ASCII string in STL format.
    /// </summary>
    public static string WriteString(Mesh mesh)
    {
      return WriteString(new Mesh[] { mesh });
    }

    /// <summary>
    /// Write a list of Rhino.Geometry.Mesh object to an ASCII string in STL format.
    /// </summary>
    public static string WriteString(IList<Mesh> meshes)
    {
      var sb = new StringBuilder();

      var name = meshes.Count == 1 ? "Mesh" : "Composite Mesh";
      sb.AppendLine(string.Format("solid {0}", name));

      foreach (var mesh in meshes)
      {
        // STL files contain triangle meshes
        var m = mesh.DuplicateMesh();
        m.Faces.ConvertQuadsToTriangles();
        m.FaceNormals.ComputeFaceNormals();
        m.Normals.ComputeNormals();
        m.Compact();

        for (var i = 0; i < m.Faces.Count; i++)
        {
          var f = m.Faces[i];

          var n = m.FaceNormals[i];
          sb.AppendLine($"facet normal {n.X} {n.Y} {n.Z}");
          sb.AppendLine("outer loop");

          var a = m.Vertices[f.A];
          var b = m.Vertices[f.B];
          var c = m.Vertices[f.C];

          sb.AppendLine($"\tvertex {a.X} {a.Y} {a.Z}");
          sb.AppendLine($"\tvertex {b.X} {b.Y} {b.Z}");
          sb.AppendLine($"\tvertex {c.X} {c.Y} {c.Z}");

          sb.AppendLine("endloop");
          sb.AppendLine("endfacet");
        }
      }

      sb.AppendLine($"endsolid {name}");

      return sb.ToString();
    }
  }
}
