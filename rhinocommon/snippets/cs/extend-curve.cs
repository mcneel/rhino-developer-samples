partial class Examples
{
  public static Result ExtendCurve(RhinoDoc doc)
  {
    ObjRef[] boundary_obj_refs;
    var rc = RhinoGet.GetMultipleObjects("Select boundary objects", false, ObjectType.AnyObject, out boundary_obj_refs);
    if (rc != Result.Success)
      return rc;
    if (boundary_obj_refs == null || boundary_obj_refs.Length == 0)
      return Result.Nothing;

    var gc = new GetObject();
    gc.SetCommandPrompt("Select curve to extend");
    gc.GeometryFilter = ObjectType.Curve;
    gc.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
    gc.DisablePreSelect ();
    gc.Get();
    if (gc.CommandResult() != Result.Success)
      return gc.CommandResult();
    var curve_obj_ref = gc.Object(0);

    var curve = curve_obj_ref.Curve();
    if (curve == null) return Result.Failure;
    double t;
    if (!curve.ClosestPoint(curve_obj_ref.SelectionPoint(), out t))
      return Result.Failure;
    var curve_end = t <= curve.Domain.Mid ? CurveEnd.Start : CurveEnd.End;

    var geometry = boundary_obj_refs.Select(obj=> obj.Geometry());
    var extended_curve = curve.Extend(curve_end, CurveExtensionStyle.Line, geometry);
    if (extended_curve != null && extended_curve.IsValid)
    {
      if (!doc.Objects.Replace(curve_obj_ref.ObjectId, extended_curve))
        return Result.Failure;
      doc.Views.Redraw();
    }
    else
    {
      RhinoApp.WriteLine("No boundary object was intersected so curve not extended");
      return Result.Nothing;
    }

    return Result.Success;
  }
}
