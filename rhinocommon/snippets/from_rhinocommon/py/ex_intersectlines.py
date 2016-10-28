import Rhino
import scriptcontext

def IntersectLines():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt( "Select lines" )
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GetMultiple( 2, 2)
    if go.CommandResult()!=Rhino.Commands.Result.Success:
      return go.CommandResult()
    if go.ObjectCount!=2: return Rhino.Commands.Result.Failure

    crv0 = go.Object(0).Geometry()
    crv1 = go.Object(1).Geometry()
    if not crv0 or not crv1: return Rhino.Commands.Result.Failure

    line0 = crv0.Line
    line1 = crv1.Line
    v0 = line0.Direction
    v0.Unitize()
    v1 = line1.Direction
    v1.Unitize()

    if v0.IsParallelTo(v1)!=0:
        print "Selected lines are parallel."
        return Rhino.Commands.Result.Nothing

    rc, a, b = Rhino.Geometry.Intersect.Intersection.LineLine(line0, line1)
    if not rc:
        print "No intersection found."
        return Rhino.Commands.Result.Nothing

    pt0 = line0.PointAt(a)
    pt1 = line1.PointAt(b)
    # pt0 and pt1 should be equal, so we will only add pt0 to the document
    scriptcontext.doc.Objects.AddPoint(pt0)
    scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    IntersectLines()