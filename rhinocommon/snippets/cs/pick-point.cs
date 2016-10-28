partial class Examples
{
  public static Result PickPoint(RhinoDoc doc)
  {
    // this creates a point where the mouse is clicked.
    var gp = new GetPoint();
    gp.SetCommandPrompt("Click for new point");
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var point3d = gp.Point();
    doc.Objects.AddPoint(point3d);
    doc.Views.Redraw();

    // selects a point that already exists
    ObjRef obj_ref;
    var rc = RhinoGet.GetOneObject("Select point", false, ObjectType.Point, out obj_ref);
    if (rc != Result.Success)
      return rc;
    var point = obj_ref.Point();
    RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}",
      point.Location.X,
      point.Location.Y,
      point.Location.Z);
    doc.Objects.UnselectAll();

    // selects multiple points that already exist
    ObjRef[] obj_refs;
    rc = RhinoGet.GetMultipleObjects("Select point", false, ObjectType.Point, out obj_refs);
    if (rc != Result.Success)
      return rc;
    foreach (var o_ref in obj_refs)
    {
      point = o_ref.Point();
      RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}",
        point.Location.X,
        point.Location.Y,
        point.Location.Z);
    }
    doc.Objects.UnselectAll();

    // also selects a point that already exists.
    // Used when RhinoGet doesn't provide enough control
    var go = new GetObject();
    go.SetCommandPrompt("Select point");
    go.GeometryFilter = ObjectType.Point;
    go.GetMultiple(1, 0);
    if (go.CommandResult() != Result.Success)
      return go.CommandResult();
    foreach (var o_ref in  go.Objects())
    {
      point = o_ref.Point();
      if (point != null)
        RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}",
          point.Location.X,
          point.Location.Y,
          point.Location.Z);
    }

    doc.Views.Redraw();
    return Result.Success;
  }
}
