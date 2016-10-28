partial class Examples
{
  public static Result CurveSurfaceIntersect(RhinoDoc doc)
  {
    var gs = new GetObject();
    gs.SetCommandPrompt("select brep");
    gs.GeometryFilter = ObjectType.Brep;
    gs.DisablePreSelect();
    gs.SubObjectSelect = false;
    gs.Get();
    if (gs.CommandResult() != Result.Success)
      return gs.CommandResult();
    var brep = gs.Object(0).Brep();

    var gc = new GetObject();
    gc.SetCommandPrompt("select curve");
    gc.GeometryFilter = ObjectType.Curve;
    gc.DisablePreSelect();
    gc.SubObjectSelect = false;
    gc.Get();
    if (gc.CommandResult() != Result.Success)
      return gc.CommandResult();
    var curve = gc.Object(0).Curve();

    if (brep == null || curve == null)
      return Result.Failure;

    var tolerance = doc.ModelAbsoluteTolerance;

    Point3d[] intersection_points;
    Curve[] overlap_curves;
    if (!Intersection.CurveBrep(curve, brep, tolerance, out overlap_curves, out intersection_points))
    {
      RhinoApp.WriteLine("curve brep intersection failed");
      return Result.Nothing;
    }

    foreach (var overlap_curve in overlap_curves)
      doc.Objects.AddCurve(overlap_curve);
    foreach (var intersection_point in intersection_points)
      doc.Objects.AddPoint(intersection_point);

    RhinoApp.WriteLine("{0} overlap curves, and {1} intersection points", overlap_curves.Length, intersection_points.Length);
    doc.Views.Redraw();

    return Result.Success;
  }
}
