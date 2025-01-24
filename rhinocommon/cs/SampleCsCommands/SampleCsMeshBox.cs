using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using System.Drawing;

namespace SampleCsCommands
{
  public class SampleCsMeshBox : Command
  {
    public override string EnglishName => "SampleCsMeshBox";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Point3d[] vertices = new Point3d[8];
      vertices[0] = new Point3d(10.0, 10.0, 10.0);
      vertices[1] = new Point3d(10.0, 10.0, -10.0);
      vertices[2] = new Point3d(10.0, -10.0, 10.0);
      vertices[3] = new Point3d(10.0, -10.0, -10.0);
      vertices[4] = new Point3d(-10.0, 10.0, 10.0);
      vertices[5] = new Point3d(-10.0, 10.0, -10.0);
      vertices[6] = new Point3d(-10.0, -10.0, 10.0);
      vertices[7] = new Point3d(-10.0, -10.0, -10.0);

      int[,] indices = new int[,]
      {
        { 0, 1, 5, 4 },
        { 0, 4, 6, 2 },
        { 0, 2, 3, 1 },
        { 7, 3, 2, 6},
        { 7, 6, 4, 5},
        { 7, 5, 1, 3}
      };

      Mesh[] meshes = new Mesh[6];
      for (int mi = 0; mi < 6; mi++)
      {
        Mesh mesh = new Mesh();
        for (int vi = 0; vi < 4; vi++)
          mesh.Vertices.Add(vertices[indices[mi, vi]]);
        mesh.Faces.AddFace(0, 1, 2, 3);
        mesh.FaceNormals.ComputeFaceNormals();
        mesh.Normals.ComputeNormals();
        mesh.Compact();
        meshes[mi] = mesh;
      }

      Color[] colors = new Color[]
      {
        Color.Red,
        Color.Orange,
        Color.Yellow,
        Color.Green,
        Color.Blue,
        Color.Purple
      };

      int gi = doc.Groups.Add();

      ObjectAttributes atts = new ObjectAttributes { ColorSource = ObjectColorSource.ColorFromObject };
      atts.AddToGroup(gi);

      for (int mi = 0; mi < 6; mi++)
      {
        atts.ObjectColor = colors[mi];
        doc.Objects.AddMesh(meshes[mi], atts);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
