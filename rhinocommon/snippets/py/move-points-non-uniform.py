from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input import *
from scriptcontext import doc

def RunCommand():
    rc, obj_refs = RhinoGet.GetMultipleObjects("Select points to move", False, ObjectType.Point)
    if rc != Result.Success or obj_refs == None:
        return rc

    for obj_ref in obj_refs:
        point3d = obj_ref.Point().Location
        point3d.X += 1
        point3d.Y += 1
        point3d.Z += 1
        doc.Objects.Replace(obj_ref, point3d)

    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
    RunCommand()
