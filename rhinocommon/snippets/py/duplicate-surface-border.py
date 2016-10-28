import Rhino
import scriptcontext

def DupBorder():
    filter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface", False, filter)
    if rc != Rhino.Commands.Result.Success: return rc

    rhobj = objref.Object()
    brep = objref.Brep()
    if not rhobj or not brep: return Rhino.Commands.Result.Failure
    rhobj.Select(False)
    curves = brep.DuplicateEdgeCurves(True)
    tol = scriptcontext.doc.ModelAbsoluteTolerance * 2.1
    curves = Rhino.Geometry.Curve.JoinCurves(curves, tol)
    for curve in curves:
        id = scriptcontext.doc.Objects.AddCurve(curve)
        scriptcontext.doc.Objects.Select(id)
    scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    DupBorder()
