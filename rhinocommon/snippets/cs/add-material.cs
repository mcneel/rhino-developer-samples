
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Render;
using System.Drawing;

partial class Examples
{
  public static Result AddMaterial(Rhino.RhinoDoc doc)
  {
    // This example demonstrates how to create a basic Render material.
    // This material will appear in Rhino's Materials panel. It it called a
    // basic material because it does not target any particular rendering plug-in.

    // Create a Rhino material.
    var rhino_material = new Material
    {
      Name = "Material",
      DiffuseColor  = Color.Chocolate,
      SpecularColor = Color.CadetBlue,
    };

    // Create a basic Render material from the Rhino material.
    var render_material = RenderMaterial.CreateBasicMaterial(rhino_material, doc);

    // Add the basic Render material to the document.
    doc.RenderMaterials.Add(render_material);

    // Create a sphere with the Render material assigned to it.
    var sphere = new Rhino.Geometry.Sphere(Rhino.Geometry.Plane.WorldXY, 5);
    var attr = new ObjectAttributes { RenderMaterial = render_material };
    doc.Objects.AddSphere(sphere, attr);

    // Add a sphere without the material attributes set.
    sphere.Center = new Rhino.Geometry.Point3d(10, 10, 0);
    doc.Objects.AddSphere(sphere);

    doc.Views.Redraw();

    return Result.Success;
  }
}
