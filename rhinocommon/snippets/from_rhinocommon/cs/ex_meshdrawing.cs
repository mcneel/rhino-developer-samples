using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input.Custom;
using Rhino.DocObjects;
using System.Drawing;

namespace examples_cs
{
  public class MeshDrawingCommand : Command
  {
    public override string EnglishName { get { return "csDrawMesh"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gs = new GetObject();
      gs.SetCommandPrompt("select sphere");
      gs.GeometryFilter = ObjectType.Surface;
      gs.DisablePreSelect();
      gs.SubObjectSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      Sphere sphere;
      gs.Object(0).Surface().TryGetSphere(out sphere);
      if (sphere.IsValid)
      {
        var mesh = Mesh.CreateFromSphere(sphere, 10, 10);
        if (mesh == null)
          return Result.Failure;

        var conduit = new DrawBlueMeshConduit(mesh) {Enabled = true};
        doc.Views.Redraw();

        var in_str = "";
        Rhino.Input.RhinoGet.GetString("press <Enter> to continue", true, ref in_str);

        conduit.Enabled = false;
        doc.Views.Redraw();
        return Result.Success;
      }
      else
        return Result.Failure;
    }
  }

  class DrawBlueMeshConduit : DisplayConduit
  {
    readonly Mesh m_mesh;
    readonly Color m_color;
    readonly DisplayMaterial m_material;
    readonly BoundingBox m_bbox;

    public DrawBlueMeshConduit(Mesh mesh)
    {
      // set up as much data as possible so we do the minimum amount of work possible inside
      // the actual display code
      m_mesh = mesh;
      m_color = System.Drawing.Color.Blue;
      m_material = new DisplayMaterial();
      m_material.Diffuse = m_color;
      if (m_mesh != null && m_mesh.IsValid)
        m_bbox = m_mesh.GetBoundingBox(true);
    }

    // this is called every frame inside the drawing code so try to do as little as possible
    // in order to not degrade display speed. Don't create new objects if you don't have to as this
    // will incur an overhead on the heap and garbage collection.
    protected override void CalculateBoundingBox(CalculateBoundingBoxEventArgs e)
    {
      base.CalculateBoundingBox(e);
      // Since we are dynamically drawing geometry, we needed to override
      // CalculateBoundingBox. Otherwise, there is a good chance that our
      // dynamically drawing geometry would get clipped.
 
      // Union the mesh's bbox with the scene's bounding box
      e.IncludeBoundingBox(m_bbox);
    }

    protected override void PreDrawObjects(DrawEventArgs e)
    {
      base.PreDrawObjects(e);
      var vp = e.Display.Viewport;
      if (vp.DisplayMode.EnglishName.ToLower() == "wireframe")
        e.Display.DrawMeshWires(m_mesh, m_color);
      else
        e.Display.DrawMeshShaded(m_mesh, m_material);
    }
  }
}