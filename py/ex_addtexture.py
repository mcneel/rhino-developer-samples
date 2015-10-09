import Rhino
import scriptcontext
import System.Guid
import System.Windows.Forms.DialogResult
import System.IO.File

def AddTexture():
    # Select object to add texture
    filter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter | Rhino.DocObjects.ObjectType.Mesh
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select object to add texture", False, filter)
    if rc!=Rhino.Commands.Result.Success: return rc

    rhino_object = objref.Object()
    if not rhino_object: return Rhino.Commands.Result.Failure

    # Select texture
    fd = Rhino.UI.OpenFileDialog()
    fd.Filter = "Image Files (*.bmp;*.png;*.jpg)|*.bmp;*.png;*.jpg"
    if fd.ShowDialog()!=System.Windows.Forms.DialogResult.OK:
        return Rhino.Commands.Result.Cancel

    # Verify texture
    bitmap_filename = fd.FileName
    if not System.IO.File.Exists(bitmap_filename):
        return Rhino.Commands.Result.Nothing

    # Make sure the object has it's material source set to "material_from_object"
    rhino_object.Attributes.MaterialSource = Rhino.DocObjects.ObjectMaterialSource.MaterialFromObject

    # Make sure the object has a material assigned
    material_index = rhino_object.Attributes.MaterialIndex
    if material_index<0:
        # Create a new material based on Rhino's default material
        material_index = scriptcontext.doc.Materials.Add()
        # Assign the new material (index) to the object.
        rhino_object.Attributes.MaterialIndex = material_index

    if material_index>=0:
        mat = scriptcontext.doc.Materials[material_index]
        # find the texture
        mi = mat.ToMaterialInfo()
        texture_index = mi.FindBitmapTexture()
        if texture_index>=0:
            # already had a texture, so just replace it
            mi.SetTextureFilename(texture_index, bitmap_filename)
        else:
            # Does not have a texture, so add one
            mi.AddBitmapTexture(bitmap_filename)
        scriptcontext.doc.Materials.Modify(mi, material_index, False)
        #Don't forget to update the object, if necessary
        rhino_object.CommitChanges()
        scriptcontext.doc.Views.Redraw()
        return Rhino.Commands.Result.Success
 
    return Rhino.Commands.Result.Failure

if __name__=="__main__":
    AddTexture()
