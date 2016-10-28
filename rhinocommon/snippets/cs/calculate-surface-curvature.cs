partial class Examples
{
  public static Result PrincipalCurvature(RhinoDoc doc)
  {
    ObjRef obj_ref;
    var rc = RhinoGet.GetOneObject("Select surface for curvature measurement", true,
      ObjectType.Surface, out obj_ref);
    if (rc != Result.Success)
      return rc;
    var surface = obj_ref.Surface();

    var gp = new Rhino.Input.Custom.GetPoint();
    gp.SetCommandPrompt("Select point on surface for curvature measurement");
    gp.Constrain(surface, false);
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var point_on_surface = gp.Point();

    double u, v;
    if (!surface.ClosestPoint(point_on_surface, out u, out v))
      return Result.Failure;

    var surface_curvature = surface.CurvatureAt(u, v);
    if (surface_curvature == null)
      return Result.Failure;

    RhinoApp.WriteLine("Surface curvature evaluation at parameter: ({0}, {1})", u, v);

    RhinoApp.WriteLine("  3-D Point: ({0}, {1}, {2})",
      surface_curvature.Point.X,
      surface_curvature.Point.Y,
      surface_curvature.Point.Z);

    RhinoApp.WriteLine("  3-D Normal: ({0}, {1}, {2})",
      surface_curvature.Normal.X,
      surface_curvature.Normal.Y,
      surface_curvature.Normal.Z);

    RhinoApp.WriteLine(string.Format("  Maximum principal curvature: {0} ({1}, {2}, {3})",
      surface_curvature.Kappa(0),
      surface_curvature.Direction(0).X,
      surface_curvature.Direction(0).Y,
      surface_curvature.Direction(0).Z));

    RhinoApp.WriteLine(string.Format("  Minimum principal curvature: {0} ({1}, {2}, {3})",
      surface_curvature.Kappa(1),
      surface_curvature.Direction(1).X,
      surface_curvature.Direction(1).Y,
      surface_curvature.Direction(1).Z));

    RhinoApp.WriteLine("  Gaussian curvature: {0}", surface_curvature.Gaussian);
    RhinoApp.WriteLine("  Mean curvature: {0}", surface_curvature.Mean);

    return Result.Success;
  }
}
