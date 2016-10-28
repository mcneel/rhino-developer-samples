import Rhino
import scriptcontext

def constrainedcopy():
    #get a single closed curve
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select curve")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
    go.Get()
    if go.CommandResult() != Rhino.Commands.Result.Success: return
    objref = go.Object(0)
    base_curve = objref.Curve()
    first_point = objref.SelectionPoint()
    if not base_curve or not first_point.IsValid:
        return
    isplanar, plane = base_curve.TryGetPlane()
    if not isplanar: return
    
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Offset point")
    gp.DrawLineFromPoint(first_point, True)
    line = Rhino.Geometry.Line(first_point, first_point + plane.Normal)
    gp.Constrain(line)
    gp.Get()
    if gp.CommandResult() != Rhino.Commands.Result.Success:
        return
    second_point = gp.Point()
    vec = second_point - first_point
    if vec.Length > 0.001:
        xf = Rhino.Geometry.Transform.Translation(vec)
        id = scriptcontext.doc.Objects.Transform(objref, xf, False)
        scriptcontext.doc.Views.Redraw()
        return id

if __name__=="__main__":
    constrainedcopy()
