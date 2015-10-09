partial class Examples
{
  public static Rhino.Commands.Result IntersectCurves(Rhino.RhinoDoc doc)
  {
    // Select two curves to intersect
    var go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select two curves");
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go.GetMultiple(2, 2);
    if (go.CommandResult() != Rhino.Commands.Result.Success)
      return go.CommandResult();

    // Validate input
    var curveA = go.Object(0).Curve();
    var curveB = go.Object(1).Curve();
    if (curveA == null || curveB == null)
      return Rhino.Commands.Result.Failure;

    // Calculate the intersection
    const double intersection_tolerance = 0.001;
    const double overlap_tolerance = 0.0;
    var events = Rhino.Geometry.Intersect.Intersection.CurveCurve(curveA, curveB, intersection_tolerance, overlap_tolerance);

    // Process the results
    if (events != null)
    {
      for (int i = 0; i < events.Count; i++)
      {
        var ccx_event = events[i];
        doc.Objects.AddPoint(ccx_event.PointA);
        if (ccx_event.PointA.DistanceTo(ccx_event.PointB) > double.Epsilon)
        {
          doc.Objects.AddPoint(ccx_event.PointB);
          doc.Objects.AddLine(ccx_event.PointA, ccx_event.PointB);
        }
      }
      doc.Views.Redraw();
    }
    return Rhino.Commands.Result.Success;
  }
}
