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
      // is an abstract material - one that is defined by the the owning 
      // rendering plug-in. But a basic Render material does share some
      // properties with a Rhino material.

      // This example demonstrates how to create a basic Render material.
      // This material will appear in Rhino's Materials panel. It it called
      // basic material because it does not target any particular rendering
      // plug-in. 

      // 1.) Create some Rhino materials
      var red_material = new Material
      {
        DiffuseColor = Color.Red,
        Name = "Red"
      };

      var green_material = new Material
      {
        DiffuseColor = Color.Green,
        Name = "Green"
      };

      var blue_material = new Material
      {
        DiffuseColor = Color.Blue,
        Name = "Blue"
      };

      // 2.) Create basic Render materials from Rhino materials
      var red_render_material = RenderMaterial.CreateBasicMaterial(red_material);
      var green_render_material = RenderMaterial.CreateBasicMaterial(green_material);
      var blue_render_material = RenderMaterial.CreateBasicMaterial(blue_material);

      // 3.) Add basic Render materials to the render material table
      doc.RenderMaterials.Add(red_render_material);
      doc.RenderMaterials.Add(green_render_material);
      doc.RenderMaterials.Add(blue_render_material);

      // At this point, you will see render materials show up in Rhino's
      // Materials panel. Note, RhinoDoc.Materials.Count will equal 0. This is
      // because we have not added any Rhino materials. We've only added
      // Render materials. When you assign a Render material to an object
      // or a layer, a compatible Rhino material will be added to RhinoDoc.Materials,
      // and this material will be referenced by the Render material.

      // Note, it is possible to add Rhino materials to the document. However,
      // they will not appear in the Materials panel until they are assigned 
      // to objects or layers. Here is an example of this.

      // 1.) Create a Rhino material
      var magenta_material = new Material
      {
        DiffuseColor = Color.Magenta,
        Name = "Magenta"
      };

      // 2.) Add the Rhino material to the document
      var magenta_material_index = doc.Materials.Add(magenta_material);

      // 3.) Create an object and assign the Rhino material to it.
      var sphere = new Sphere(Plane.WorldXY, 5);
      var attributes = doc.CreateDefaultAttributes();
      attributes.MaterialSource = ObjectMaterialSource.MaterialFromObject;
      attributes.MaterialIndex = magenta_material_index;
      doc.Objects.AddSphere(sphere, attributes);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}

