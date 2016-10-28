partial class Examples
{
  public static Result ExtractIsoCurve(RhinoDoc doc)
  {
    ObjRef obj_ref;
    var rc = RhinoGet.GetOneObject("Select surface", false, ObjectType.Surface, out obj_ref);
    if (rc != Result.Success || obj_ref == null)
      return rc;
    var surface = obj_ref.Surface();

    var gp = new GetPoint();
    gp.SetCommandPrompt("Point on surface");
    gp.Constrain(surface, false);
    var option_toggle = new OptionToggle(false, "U", "V");
    gp.AddOptionToggle("Direction", ref option_toggle);
    Point3d point = Point3d.Unset;
    while (true)
    {
      var grc = gp.Get();
      if (grc == GetResult.Option)
        continue;
      else if (grc == GetResult.Point)
      {
        point = gp.Point();
        break;
      }
      else
        return Result.Nothing;
    }
    if (point == Point3d.Unset)
      return Result.Nothing;

    int direction = option_toggle.CurrentValue ? 1 : 0; // V : U
    double u_parameter, v_parameter;
    if (!surface.ClosestPoint(point, out u_parameter, out v_parameter)) return Result.Failure;

    var iso_curve = surface.IsoCurve(direction, direction == 1 ? u_parameter : v_parameter);
    if (iso_curve == null) return Result.Failure;

    doc.Objects.AddCurve(iso_curve);
    doc.Views.Redraw();
    return Result.Success;
  }
}
