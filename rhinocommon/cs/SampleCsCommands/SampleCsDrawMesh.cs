
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsDrawMeshConduit : Rhino.Display.DisplayConduit
  {
    public Mesh Mesh { get; private set; }
    public DisplayMaterial Material { get; private set; }
    
    public SampleCsDrawMeshConduit(Mesh mesh, Material material)
    {
      Mesh = mesh;
      Material = new DisplayMaterial(material);
    }

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
        e.Display.DrawMeshShaded(Mesh, Material);
        e.Display.DrawMeshWires(Mesh, System.Drawing.Color.Black);
      }
    }
  }

  public class SampleCsDrawMesh : Command
  {
    public override string EnglishName => "SampleCsDrawMesh";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var sphere = new Sphere(Plane.WorldXY, 10.0);
      var mesh = Mesh.CreateFromSphere(sphere, 10, 10);
      var material = doc.Materials[-1]; // -1 is out of range; the current material is returned.

      var conduit = new SampleCsDrawMeshConduit(mesh, material)
      {
        Enabled = true
      };
      doc.Views.Redraw();

      string out_str = null;
      Rhino.Input.RhinoGet.GetString("Press <Enter> to continue", true, ref out_str);

      conduit.Enabled = false;

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
