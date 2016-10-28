partial class Examples
{
  public static Result MovePointObjectsNonUniform(RhinoDoc doc)
  {
    ObjRef[] obj_refs;
    var rc = RhinoGet.GetMultipleObjects("Select points to move", false, ObjectType.Point, out obj_refs);
    if (rc != Result.Success || obj_refs == null)
      return rc;

    foreach (var obj_ref in obj_refs)
    {
      var point3d = obj_ref.Point().Location;
      // modify the point coordinates in some way ...
      point3d.X++;
      point3d.Y++;
      point3d.Z++;

      doc.Objects.Replace(obj_ref, point3d);
    }

    doc.Views.Redraw();
    return Result.Success;
  }
}
