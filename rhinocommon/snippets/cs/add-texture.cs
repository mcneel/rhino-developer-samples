partial class Examples
{
  public static Rhino.Commands.Result AddTexture(Rhino.RhinoDoc doc)
  {
    // Select object to add texture
    const ObjectType filter = Rhino.DocObjects.ObjectType.Surface |
                              Rhino.DocObjects.ObjectType.PolysrfFilter |
                              Rhino.DocObjects.ObjectType.Mesh;
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select object to add texture", false, filter, out objref);
    if( rc!= Rhino.Commands.Result.Success )
      return rc;

    Rhino.DocObjects.RhinoObject rhino_object = objref.Object();
    if (rhino_object == null)
      return Rhino.Commands.Result.Failure;

    // Select texture
    Rhino.UI.OpenFileDialog fd = new Rhino.UI.OpenFileDialog();
    fd.Filter = "Image Files (*.bmp;*.png;*.jpg)|*.bmp;*.png;*.jpg";
    if (!fd.ShowDialog())
      return Rhino.Commands.Result.Cancel;

    // Verify texture
    string bitmap_filename = fd.FileName;
    if( string.IsNullOrEmpty(bitmap_filename) || !System.IO.File.Exists(bitmap_filename) )
      return Rhino.Commands.Result.Nothing;

    // Make sure the object has it's material source set to "material_from_object"
    rhino_object.Attributes.MaterialSource = Rhino.DocObjects.ObjectMaterialSource.MaterialFromObject;

    // Make sure the object has a material assigned
    int material_index = rhino_object.Attributes.MaterialIndex;
    if (material_index < 0)
    {
      // Create a new material based on Rhino's default material
      material_index = doc.Materials.Add();
      // Assign the new material (index) to the object.
      rhino_object.Attributes.MaterialIndex = material_index;
    }

    if (material_index >= 0)
    {
      Rhino.DocObjects.Material mat = doc.Materials[material_index];
      mat.SetBumpTexture(bitmap_filename);
      mat.CommitChanges();

      //Don't forget to update the object, if necessary
      rhino_object.CommitChanges();

      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }

    return Rhino.Commands.Result.Failure;
  }
}
