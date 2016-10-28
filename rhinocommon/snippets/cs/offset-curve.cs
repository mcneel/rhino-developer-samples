partial class Examples
{
  public static Result OffsetCurve(RhinoDoc doc)
  {
    ObjRef obj_ref;
    var rs = RhinoGet.GetOneObject(
      "Select Curve", false, ObjectType.Curve, out obj_ref);
    if (rs != Result.Success)
      return rs;
    var curve = obj_ref.Curve();
    if (curve == null)
      return Result.Nothing;

    Point3d point;
    rs = RhinoGet.GetPoint("Select Side", false, out point);
    if (rs != Result.Success)
      return rs;
    if (point == Point3d.Unset)
      return Result.Nothing;

    var curves = curve.Offset(point, Vector3d.ZAxis, 1.0,
      doc.ModelAbsoluteTolerance, CurveOffsetCornerStyle.None);

    foreach (var offset_curve in curves)
      doc.Objects.AddCurve(offset_curve);

    doc.Views.Redraw();
    return Result.Success;
  }
}
