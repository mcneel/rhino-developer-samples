using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsColorfulMeshBox : Command
  {
    private Random m_random;

    public SampleCsColorfulMeshBox()
    {
      m_random = new Random();
    }

    public override string EnglishName
    {
      get { return "SampleCsColorfulMeshBox"; }
    }

    private System.Drawing.Color GetRandomColor()
    {
      return System.Drawing.Color.FromArgb(m_random.Next(256), m_random.Next(256), m_random.Next(256));
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Geometry.Mesh mesh = new Rhino.Geometry.Mesh();

      mesh.Vertices.Add(new Rhino.Geometry.Point3d(0.5,  0.5,  0.5));
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

      // For vertex colors to "work", you must supply a color
      // for every vertex.
      for (int i = 0; i < mesh.Vertices.Count; i++)
        mesh.VertexColors.Add(GetRandomColor());

      mesh.FaceNormals.ComputeFaceNormals();
      mesh.Normals.ComputeNormals();
      mesh.Compact();

      doc.Objects.AddMesh(mesh);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
