partial class Examples
{
  private static double m_tolerance;
  public static Result CustomGeometryFilter(RhinoDoc doc)
  {
    m_tolerance = doc.ModelAbsoluteTolerance;

    // only use a custom geometry filter if no simpler filter does the job

    // only curves
    var gc = new GetObject();
    gc.SetCommandPrompt("select curve");
    gc.GeometryFilter = ObjectType.Curve;
    gc.DisablePreSelect();
    gc.SubObjectSelect = false;
    gc.Get();
    if (gc.CommandResult() != Result.Success)
      return gc.CommandResult();
    if (null == gc.Object(0).Curve())
      return Result.Failure;
    Rhino.RhinoApp.WriteLine("curve was selected");

    // only closed curves
    var gcc = new GetObject();
    gcc.SetCommandPrompt("select closed curve");
    gcc.GeometryFilter = ObjectType.Curve;
    gcc.GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve;
    gcc.DisablePreSelect();
    gcc.SubObjectSelect = false;
    gcc.Get();
    if (gcc.CommandResult() != Result.Success)
      return gcc.CommandResult();
    if (null == gcc.Object(0).Curve())
      return Result.Failure;
    Rhino.RhinoApp.WriteLine("closed curve was selected");

    // only circles with a radius of 10
    var gcc10 = new GetObject();
    gcc10.SetCommandPrompt("select circle with radius of 10");
    gc.GeometryFilter = ObjectType.Curve;
    gcc10.SetCustomGeometryFilter(CircleWithRadiusOf10GeometryFilter); // custom geometry filter
    gcc10.DisablePreSelect();
    gcc10.SubObjectSelect = false;
    gcc10.Get();
    if (gcc10.CommandResult() != Result.Success)
      return gcc10.CommandResult();
    if (null == gcc10.Object(0).Curve())
      return Result.Failure;
    RhinoApp.WriteLine("circle with radius of 10 was selected");

    return Result.Success;
  }

  private static bool CircleWithRadiusOf10GeometryFilter (Rhino.DocObjects.RhinoObject rhObject, GeometryBase geometry,
    ComponentIndex componentIndex)
  {
    bool is_circle_with_radius_of10 = false;
    Circle circle;
    if (geometry is Curve && (geometry as Curve).TryGetCircle(out circle))
      is_circle_with_radius_of10 = circle.Radius <= 10.0 + m_tolerance && circle.Radius >= 10.0 - m_tolerance;
    return is_circle_with_radius_of10;
  }
}
