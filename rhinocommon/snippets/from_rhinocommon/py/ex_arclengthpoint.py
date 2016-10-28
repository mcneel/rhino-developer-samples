import Rhino
import scriptcontext

def ArcLengthPoint():
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select curve", True, Rhino.DocObjects.ObjectType.Curve)
    if rc!=Rhino.Commands.Result.Success: return rc
    crv = objref.Curve()
    if not crv: return Rhino.Commands.Result.Failure
    crv_length = crv.GetLength()
    length = 0
    rc, length = Rhino.Input.RhinoGet.GetNumber("Length from start", True, length, 0, crv_length)
    if rc!=Rhino.Commands.Result.Success: return rc
    pt = crv.PointAtLength(length)
    if pt.IsValid:
        scriptcontext.doc.Objects.AddPoint(pt)
        scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    ArcLengthPoint()
