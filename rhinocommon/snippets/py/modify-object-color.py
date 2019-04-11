from System.Drawing import *
from Rhino import *
from Rhino.DocObjects import *
from Rhino.Geometry import *
from Rhino.Input import *
from Rhino.Commands import *
from Rhino.UI.Dialogs import ShowColorDialog
from scriptcontext import doc

def RunCommand():
    rc, obj_ref = RhinoGet.GetOneObject("Select object", False, ObjectType.AnyObject)
    if rc != Result.Success:
        return rc
    rhino_object = obj_ref.Object()
    color = rhino_object.Attributes.ObjectColor
    b, color = ShowColorDialog(color)
    if not b: return Result.Cancel

    rhino_object.Attributes.ObjectColor = color
    rhino_object.Attributes.ColorSource = ObjectColorSource.ColorFromObject
    rhino_object.CommitChanges()

    # an object's color attributes can also be specified
    # when the object is added to Rhino
    sphere = Sphere(Point3d.Origin, 5.0)
    attributes = ObjectAttributes()
    attributes.ObjectColor = Color.CadetBlue
    attributes.ColorSource = ObjectColorSource.ColorFromObject
    doc.Objects.AddSphere(sphere, attributes)

    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
    RunCommand()
