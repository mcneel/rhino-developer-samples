import Rhino
import scriptcontext

def CircleCenter():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select objects")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
    go.GetMultiple(1, 0)
    if go.CommandResult()!=Rhino.Commands.Result.Success:
        return go.CommandResult()

    objrefs = go.Objects()
    if not objrefs: return Rhino.Commands.Result.Nothing

    tolerance = scriptcontext.doc.ModelAbsoluteTolerance
    for i, objref in enumerate(objrefs):
        # get the curve geometry
        curve = objref.Curve()
        if not curve: continue
        rc, circle = curve.TryGetCircle( tolerance )
        if rc:
            print "Circle", i+1, ": center = ", circle.Center
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    CircleCenter()