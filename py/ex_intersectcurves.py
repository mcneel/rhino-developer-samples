import Rhino
import scriptcontext

def IntersectCurves():
    # Select two curves to intersect
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select two curves")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GetMultiple(2, 2)
    if go.CommandResult()!=Rhino.Commands.Result.Success: return

    # Validate input
    curveA = go.Object(0).Curve()
    curveB = go.Object(1).Curve()
    if not curveA or not curveB: return

    # Calculate the intersection
    intersection_tolerance = 0.001
    overlap_tolerance = 0.0
    events = Rhino.Geometry.Intersect.Intersection.CurveCurve(curveA, curveB, intersection_tolerance, overlap_tolerance)

    # Process the results
    if not events: return
    for ccx_event in events:
        scriptcontext.doc.Objects.AddPoint(ccx_event.PointA)
        if ccx_event.PointA.DistanceTo(ccx_event.PointB) > float.Epsilon:
            scriptcontext.doc.Objects.AddPoint(ccx_event.PointB)
            scriptcontext.doc.Objects.AddLine(ccx_event.PointA, ccx_event.PointB)
    scriptcontext.doc.Views.Redraw()

if __name__=="__main__":
    IntersectCurves()