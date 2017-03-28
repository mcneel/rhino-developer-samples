using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsDrawMeshConduit : Rhino.Display.DisplayConduit
  {
    public Rhino.Geometry.Mesh Mesh { get; set; }

    protected override void CalculateBoundingBox(Rhino.Display.CalculateBoundingBoxEventArgs e)
    {
      if (null != Mesh)
      {
        e.IncludeBoundingBox(Mesh.GetBoundingBox(false));
      }
    }

    protected override void PostDrawObjects(Rhino.Display.DrawEventArgs e)
    {
      if (null != Mesh)
      {
        Rhino.Display.DisplayMaterial material = new Rhino.Display.DisplayMaterial();
        material.IsTwoSided = true;
        material.Diffuse = System.Drawing.Color.Blue;
        material.BackDiffuse = System.Drawing.Color.Red;
        e.Display.EnableLighting(true);
        e.Display.DrawMeshShaded(Mesh, material);
        e.Display.DrawMeshWires(Mesh, System.Drawing.Color.Black);
      }
    }
  }

  [System.Runtime.InteropServices.Guid("afe4262d-b092-40f4-8dfd-f82ecd2e69ab")]
  public class SampleCsDrawMesh : Command
  {
    public SampleCsDrawMesh()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsDrawMesh"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Geometry.Point3d[] corners;
      Result rc = Rhino.Input.RhinoGet.GetRectangle(out corners);
      if (rc != Result.Success)
        return rc;

      Rhino.Geometry.Plane plane = new Rhino.Geometry.Plane(corners[0], corners[1], corners[2]);
      Rhino.Geometry.Interval x_interval = new Rhino.Geometry.Interval(0, corners[0].DistanceTo(corners[1]));
      Rhino.Geometry.Interval y_interval = new Rhino.Geometry.Interval(0, corners[1].DistanceTo(corners[2]));

      Rhino.Geometry.Mesh mesh = Rhino.Geometry.Mesh.CreateFromPlane(plane, x_interval, y_interval, 10, 10);
      //mesh.FaceNormals.ComputeFaceNormals();
      //mesh.Normals.ComputeNormals();

      SampleCsDrawMeshConduit conduit = new SampleCsDrawMeshConduit();
      conduit.Mesh = mesh;
      conduit.Enabled = true;
      doc.Views.Redraw();

      string out_str = null;
      rc = Rhino.Input.RhinoGet.GetString("Press <Enter> to continue", true, ref out_str);

      conduit.Enabled = false;

      doc.Views.Redraw();

      return Result.Success;

    }
  }
}
