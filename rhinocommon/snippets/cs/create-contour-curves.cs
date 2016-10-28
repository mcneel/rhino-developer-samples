partial class Examples
{
  public static Result ContourCurves(RhinoDoc doc)
  {
    var filter = ObjectType.Surface | ObjectType.PolysrfFilter | ObjectType.Mesh;
    ObjRef[] obj_refs;
    var rc = RhinoGet.GetMultipleObjects("Select objects to contour", false, filter, out obj_refs);
    if (rc != Result.Success)
      return rc;

    var gp = new GetPoint();
    gp.SetCommandPrompt("Contour plane base point");
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var base_point = gp.Point();

    gp.DrawLineFromPoint(base_point, true);
    gp.SetCommandPrompt("Direction perpendicular to contour planes");
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var end_point = gp.Point();

    if (base_point.DistanceTo(end_point) < RhinoMath.ZeroTolerance)
      return Result.Nothing;

    double distance = 1.0;
    rc = RhinoGet.GetNumber("Distance between contours", false, ref distance);
    if (rc != Result.Success)
      return rc;

    var interval = Math.Abs(distance);

    Curve[] curves = null;
    foreach (var obj_ref in obj_refs)
    {
      var geometry = obj_ref.Geometry();
      if (geometry == null)
        return Result.Failure;

      if (geometry is Brep)
      {
        curves = Brep.CreateContourCurves(geometry as Brep, base_point, end_point, interval);
      }
      else
      {
        curves = Mesh.CreateContourCurves(geometry as Mesh, base_point, end_point, interval);
      }

      foreach (var curve in curves)
      {
        var curve_object_id = doc.Objects.AddCurve(curve);
        doc.Objects.Select(curve_object_id);
      }
    }

    if (curves != null)
      doc.Views.Redraw();
    return Result.Success;
  }
}
