using System;
using System.Drawing;
using System.Text;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.FileIO;
using Rhino.Geometry;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("cf607c82-f54e-4648-a11c-12d07c9cb444")]
  public class SampleCsMeshBox : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsMeshBox"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var vertices = new Point3d[8];
      vertices[0] = new Point3d(10.0, 10.0, 10.0);
      vertices[1] = new Point3d(10.0, 10.0, -10.0);
      vertices[2] = new Point3d(10.0, -10.0, 10.0);
      vertices[3] = new Point3d(10.0, -10.0, -10.0);
      vertices[4] = new Point3d(-10.0, 10.0, 10.0);
      vertices[5] = new Point3d(-10.0, 10.0, -10.0);
      vertices[6] = new Point3d(-10.0, -10.0, 10.0);
      vertices[7] = new Point3d(-10.0, -10.0, -10.0);

      var indices = new int[,]
      {
        { 0, 1, 5, 4 }, 
        { 0, 4, 6, 2 }, 
        { 0, 2, 3, 1 }, 
        { 7, 3, 2, 6}, 
        { 7, 6, 4, 5}, 
        { 7, 5, 1, 3}
      };

      var meshes = new Mesh[6];
      for (var mi = 0; mi < 6; mi++)
      {
        var mesh = new Mesh();
        for (var vi = 0; vi < 4; vi++)
          mesh.Vertices.Add(vertices[indices[mi, vi]]);
        mesh.Faces.AddFace(0, 1, 2, 3);
        mesh.FaceNormals.ComputeFaceNormals();
        mesh.Normals.ComputeNormals();
        mesh.Compact();
        meshes[mi] = mesh;
      }

      var colors = new Color[]
      {
        Color.Red,
        Color.Orange,
        Color.Yellow,
        Color.Green,
        Color.Blue,
        Color.Purple
      };

      var gi = doc.Groups.Add();

      var atts = new ObjectAttributes {ColorSource = ObjectColorSource.ColorFromObject};
      atts.AddToGroup(gi);

      for (var mi = 0; mi < 6; mi++)
      {
        atts.ObjectColor = colors[mi];
        doc.Objects.AddMesh(meshes[mi], atts);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
