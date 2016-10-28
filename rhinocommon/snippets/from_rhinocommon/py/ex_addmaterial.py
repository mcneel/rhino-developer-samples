import Rhino
import scriptcontext
import System.Drawing

def AddMaterial():
    # materials are stored in the document's material table
    index = scriptcontext.doc.Materials.Add()
    mat = scriptcontext.doc.Materials[index]
    mat.DiffuseColor = System.Drawing.Color.Chocolate
    mat.SpecularColor = System.Drawing.Color.CadetBlue
    mat.CommitChanges()

    # set up object attributes to say they use a specific material
    sp = Rhino.Geometry.Sphere(Rhino.Geometry.Plane.WorldXY, 5)
    attr = Rhino.DocObjects.ObjectAttributes()
    attr.MaterialIndex = index
    attr.MaterialSource = Rhino.DocObjects.ObjectMaterialSource.MaterialFromObject
    scriptcontext.doc.Objects.AddSphere(sp, attr)

    # add a sphere without the material attributes set
    sp.Center = Rhino.Geometry.Point3d(10, 10, 0)
    scriptcontext.doc.Objects.AddSphere(sp)

    scriptcontext.doc.Views.Redraw();

if __name__=="__main__":
    AddMaterial()