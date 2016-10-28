partial class Examples
{
  public static Result NurbsSurfaceIncreaseDegree(RhinoDoc doc)
  {
    ObjRef obj_ref;
    var rc = RhinoGet.GetOneObject(
      "Select surface", false, ObjectType.Surface, out obj_ref);
    if (rc != Result.Success) return rc;
    if (obj_ref == null) return Result.Failure;
    var surface = obj_ref.Surface();
    if (surface == null) return Result.Failure;
    var nurbs_surface = surface.ToNurbsSurface();

    int new_u_degree = -1;
    rc = RhinoGet.GetInteger(string.Format("New U degree <{0}...11>", nurbs_surface.Degree(0)), true, ref new_u_degree,
      nurbs_surface.Degree(0), 11);
    if (rc != Result.Success) return rc;

    int new_v_degree = -1;
    rc = RhinoGet.GetInteger(string.Format("New V degree <{0}...11>", nurbs_surface.Degree(1)), true, ref new_v_degree,
      nurbs_surface.Degree(1), 11);
    if (rc != Result.Success) return rc;

    rc = Result.Failure;
    if (nurbs_surface.IncreaseDegreeU(new_u_degree))
      if (nurbs_surface.IncreaseDegreeV(new_v_degree))
        if (doc.Objects.Replace(obj_ref.ObjectId, nurbs_surface))
          rc = Result.Success;

    RhinoApp.WriteLine("Result: {0}", rc.ToString());
    doc.Views.Redraw();
    return rc;
  }
}
