
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Render;
using System.Drawing;

partial class Examples
{
  public static Result AddTexture(Rhino.RhinoDoc doc)
  {
    // This example shows how to assign a render texture to an object. The texture is created as a child
    // of a material in the bitmap-texture child slot. The texture slot is also switched on, and a blend
    // amount of 80% is set on the texture slot. This last part is optional; the default is 100%.

    // Select object to add texture.
    const ObjectType filter = ObjectType.Surface | ObjectType.PolysrfFilter | ObjectType.Mesh;
    var rc = Rhino.Input.RhinoGet.GetOneObject("Select object to add texture", false, filter, out ObjRef objref);
    if (rc != Result.Success)
      return rc;

    var rhino_object = objref.Object();
    if (rhino_object == null)
      return Result.Failure;

    // Create a Rhino material.
    var rhino_material = new Material
    {
      Name = "Material",
      DiffuseColor = Color.White
    };

    // Create a basic Render material from the Rhino material.
    var render_material = RenderMaterial.CreateBasicMaterial(rhino_material, doc);

    // Add the basic Render material to the document.
    doc.RenderMaterials.Add(render_material);

    // We will be adding a texture to the material's bitmap-texture child slot.
    var child_slot_name = "bitmap-texture";
    var cc = RenderContent.ChangeContexts.Program;

    // Create a bitmap texture. This will also prompt for a file name.
    RenderContent.Create(doc, ContentUuids.BitmapTextureType, render_material, child_slot_name);

    // Turn on the bitmap-texture child slot of the render material and set a blend amount of 80%.
    render_material.BeginChange(cc);
    render_material.SetChildSlotOn(child_slot_name, true, cc);
    render_material.SetChildSlotAmount(child_slot_name, 80.0, cc);
    render_material.EndChange();

    // Assign the render material to the object.
    rhino_object.RenderMaterial = render_material;
    rhino_object.CommitChanges();

    // Uodate the viewports.
    doc.Views.Redraw();

    return Result.Success;
  }
}
