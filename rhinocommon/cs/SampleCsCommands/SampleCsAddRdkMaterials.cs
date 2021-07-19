using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Render;

namespace SampleCsCommands
{
  public class SampleCsAddRdkMaterials : Command
  {
    public override string EnglishName => "SampleCsAddRdkMaterials";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // The properties of a Rhino material (Rhino.DocObjects.Material) define
      // what is required to draw an object using OpenGL. Most rendering plug-ins
      // require a much richer definition than this. And, not all rendering
      // plug-ins support the same material properties. This is where Render
      // materials (Rhino.Render.RenderMaterial) come in. A Render material
      // is an abstract material - one that is defined by the owning
      // rendering plug-in. But a basic Render material does share some
      // properties with a Rhino material.

      // This example demonstrates how to create a basic Render material.
      // This material will appear in Rhino's Materials panel. It it called a
      // basic material because it does not target any particular rendering plug-in.

      // 1. Create a Rhino material.
      var rhino_material = new Material
      {
        Name = "Burly",
        DiffuseColor = Color.BurlyWood
      };

      // 2. Create a basic Render material from the Rhino material.
      var render_material = RenderMaterial.CreateBasicMaterial(rhino_material, doc);

      // 3. Add the basic Render material to the document.
      doc.RenderMaterials.Add(render_material);

      // At this point, you will see a Render material appear in Rhino's
      // Materials panel. Note, RhinoDoc.Materials.Count will equal 0. This is
      // because we have not added a Rhino material. We've only added a
      // Render material. When you assign a Render material to an object
      // or layer, a compatible Rhino material will be added to RhinoDoc.Materials,
      // and this material will be referenced by the Render material.

      // NOTE: It is still possible to add plain Rhino materials to the document.
      // However, this is no longer recommended because Render materials should always be used.

      // 4. Now create a sphere with the Render material assigned to it.
      var sphere = new Sphere(Plane.WorldXY, 5);
      var attr = new ObjectAttributes { RenderMaterial = render_material };
      doc.Objects.AddSphere(sphere, attr);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
