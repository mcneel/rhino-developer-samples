partial class Examples
{
  public static Result FurthestZOnSurfaceGivenXY(RhinoDoc doc)
  {
    #region user input
    // select a surface
    var gs = new GetObject();
    gs.SetCommandPrompt("select surface");
    gs.GeometryFilter = ObjectType.Surface;
    gs.DisablePreSelect();
    gs.SubObjectSelect = false;
    gs.Get();
    if (gs.CommandResult() != Result.Success)
      return gs.CommandResult();
    // get the brep
    var brep = gs.Object(0).Brep();
    if (brep == null)
      return Result.Failure;

    // get X and Y
    double x = 0.0, y = 0.0;
    var rc = RhinoGet.GetNumber("value of X coordinate", true, ref x);
    if (rc != Result.Success)
      return rc;
    rc = RhinoGet.GetNumber("value of Y coordinate", true, ref y);
    if (rc != Result.Success)
      return rc;
    #endregion

    // an earlier version of this sample used a curve-brep intersection to find Z
    //var maxZ = maxZIntersectionMethod(brep, x, y, doc.ModelAbsoluteTolerance);

    // projecting points is another way to find Z
    var max_z = MaxZProjectionMethod(brep, x, y, doc.ModelAbsoluteTolerance);

    if (max_z != null)
    {
      RhinoApp.WriteLine("Maximum surface Z coordinate at X={0}, Y={1} is {2}", x, y, max_z);
      doc.Objects.AddPoint(new Point3d(x, y, max_z.Value));
      doc.Views.Redraw();
    }
    else
      RhinoApp.WriteLine("no maximum surface Z coordinate at X={0}, Y={1} found.", x, y);

    return Result.Success;
  }

  private static double? MaxZProjectionMethod(Brep brep, double x, double y, double tolerance)
  {
    double? max_z = null;
    var breps = new List<Brep> {brep};
    var points = new List<Point3d> {new Point3d(x, y, 0)};
    // grab all the points projected in Z dir.  Aggregate finds furthest Z from XY plane
    try {
      max_z = (from pt in Intersection.ProjectPointsToBreps(breps, points, new Vector3d(0, 0, 1), tolerance) select pt.Z)
              // Here you might be tempted to use .Max() to get the largest Z value but that doesn't work because
              // Z might be negative.  This custom aggregate returns the max Z independant of the sign.  If it had a name
              // it could be MaxAbs()
              .Aggregate((z1, z2) => Math.Abs(z1) > Math.Abs(z2) ? z1 : z2);
    } catch (InvalidOperationException) {/*Sequence contains no elements*/}
    return max_z;
  }

  private static double? MaxZIntersectionMethod(Brep brep, double x, double y, double tolerance)
  {
    double? max_z = null;

    var bbox = brep.GetBoundingBox(true);
    var max_dist_from_xy = (from corner in bbox.GetCorners() select corner.Z)
                            // furthest Z from XY plane.
                            // Here you might be tempted to use .Max() to get the largest Z value but that doesn't work because
                            // Z might be negative.  This custom aggregate returns the max Z independant of the sign.  If it had a name
                            // it could be MaxAbs()
                            .Aggregate((z1, z2) => Math.Abs(z1) > Math.Abs(z2) ? z1 : z2);
    // multiply distance by 2 to make sure line intersects completely
    var line_curve = new LineCurve(new Point3d(x, y, 0), new Point3d(x, y, max_dist_from_xy*2));

    Curve[] overlap_curves;
    Point3d[] inter_points;
    if (Intersection.CurveBrep(line_curve, brep, tolerance, out overlap_curves, out inter_points))
    {
      if (overlap_curves.Length > 0 || inter_points.Length > 0)
      {
        // grab all the points resulting frem the intersection.
        //    1st set: points from overlapping curves,
        //    2nd set: points when there was no overlap
        //    .Aggregate: furthest Z from XY plane.
        max_z = (from c in overlap_curves select Math.Abs(c.PointAtEnd.Z) > Math.Abs(c.PointAtStart.Z) ? c.PointAtEnd.Z : c.PointAtStart.Z)
                .Union
                (from p in inter_points select p.Z)
                 // Here you might be tempted to use .Max() to get the largest Z value but that doesn't work because
                 // Z might be negative.  This custom aggregate returns the max Z independant of the sign.  If it had a name
                 // it could be MaxAbs()
                .Aggregate((z1, z2) => Math.Abs(z1) > Math.Abs(z2) ? z1 : z2);
      }
    }
    return max_z;
  }
}
