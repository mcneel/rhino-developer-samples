partial class Examples
{
  public static Result GetAngle(RhinoDoc doc)
  {
    var gp = new GetPoint();
    gp.SetCommandPrompt("Base point");
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var base_point = gp.Point();

    gp.SetCommandPrompt("First reference point");
    gp.DrawLineFromPoint(base_point, true);
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var first_point = gp.Point();

    double angle_radians;
    var rc = RhinoGet.GetAngle("Second reference point", base_point, first_point, 0, out angle_radians);
    if (rc == Result.Success)
      RhinoApp.WriteLine("Angle = {0} degrees", RhinoMath.ToDegrees(angle_radians));

    return rc;
  }
}
