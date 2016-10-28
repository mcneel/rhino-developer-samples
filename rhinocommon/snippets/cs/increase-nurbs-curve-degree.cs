partial class Examples
{
  public static Result NurbsCurveIncreaseDegree(RhinoDoc doc)
  {
    ObjRef obj_ref;
    var rc = RhinoGet.GetOneObject(
      "Select curve", false, ObjectType.Curve, out obj_ref);
    if (rc != Result.Success) return rc;
    if (obj_ref == null) return Result.Failure;
    var curve = obj_ref.Curve();
    if (curve == null) return Result.Failure;
    var nurbs_curve = curve.ToNurbsCurve();

    int new_degree = -1;
    rc = RhinoGet.GetInteger(string.Format("New degree <{0}...11>", nurbs_curve.Degree), true, ref new_degree,
      nurbs_curve.Degree, 11);
    if (rc != Result.Success) return rc;

    rc = Result.Failure;
    if (nurbs_curve.IncreaseDegree(new_degree))
      if (doc.Objects.Replace(obj_ref.ObjectId, nurbs_curve))
        rc = Result.Success;

    RhinoApp.WriteLine("Result: {0}", rc.ToString());
    doc.Views.Redraw();
    return rc;
  }
}
