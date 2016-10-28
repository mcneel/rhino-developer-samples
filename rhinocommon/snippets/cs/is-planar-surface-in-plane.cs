partial class Examples
{
  public static Result IsPlanarSurfaceInPlane(RhinoDoc doc)
  {
    ObjRef obj_ref;
    var rc = RhinoGet.GetOneObject("select surface", true, ObjectType.Surface, out obj_ref);
    if (rc != Result.Success)
      return rc;
    var surface = obj_ref.Surface();

    Point3d[] corners;
    rc = RhinoGet.GetRectangle(out corners);
    if (rc != Result.Success)
      return rc;

    var plane = new Plane(corners[0], corners[1], corners[2]);

    var is_or_isnt = " not ";
    if (IsSurfaceInPlane(surface, plane, doc.ModelAbsoluteTolerance))
      is_or_isnt = "";

    RhinoApp.WriteLine("Surface is{0} in plane.", is_or_isnt);
    return Result.Success;
  }

  private static bool IsSurfaceInPlane(Surface surface, Plane plane, double tolerance)
  {
    if (!surface.IsPlanar(tolerance))
      return false;

    var bbox = surface.GetBoundingBox(true);
    return bbox.GetCorners().All(c => System.Math.Abs(plane.DistanceTo(c)) <= tolerance);
  }
}
