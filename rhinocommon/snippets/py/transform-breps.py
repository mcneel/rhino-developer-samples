import Rhino
import scriptcontext

def TransformBrep():
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select brep", True, Rhino.DocObjects.ObjectType.Brep)
    if rc!=Rhino.Commands.Result.Success: return

    # Simple translation transformation
    xform = Rhino.Geometry.Transform.Translation(18,-18,25)
    scriptcontext.doc.Objects.Transform(objref, xform, True)
    scriptcontext.doc.Views.Redraw()

if __name__=="__main__":
    TransformBrep()
