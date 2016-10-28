import Rhino
import scriptcontext

def UnrollSurface():
    filter = Rhino.DocObjects.ObjectType.Brep | Rhino.DocObjects.ObjectType.Surface
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select surface or brep to unroll", False, filter)
    if rc!=Rhino.Commands.Result.Success: return rc;

    unroll = Rhino.Geometry.Unroller(objref.Geometry())
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select points, curves, and dots to unroll with surface")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Point | Rhino.DocObjects.ObjectType.Curve | Rhino.DocObjects.ObjectType.TextDot
    go.AcceptNothing(True)
    go.GetMultiple(0, 0)
    if go.CommandResult()!=Rhino.Commands.Result.Success:
        return go.CommandResult()

    for i in range(go.ObjectCount):
        objref = go.Object(i);
        g = objref.Geometry();
        unroll.AddFollowingGeometry(g)

    unroll.ExplodeOutput = False
    breps, curves, points, dots = unroll.PerformUnroll()
    if not breps: return Rhino.Commands.Result.Failure
    for brep in breps: scriptcontext.doc.Objects.AddBrep(brep)
    for curve in curves: scriptcontext.doc.Objects.AddCurve(curve)
    for point in points: scriptcontext.doc.Objects.AddPoint(point)
    for dot in dots: scriptcontext.doc.Objects.AddTextDot(dot)
    scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    UnrollSurface()
