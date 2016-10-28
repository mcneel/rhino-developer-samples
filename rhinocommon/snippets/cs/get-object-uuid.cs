partial class Examples
{
  public static Result GetUUID(RhinoDoc doc)
  {
    ObjRef obj_ref;
    var rc = RhinoGet.GetOneObject("Select object", false, ObjectType.AnyObject, out obj_ref);
    if (rc != Result.Success)
      return rc;
    if (obj_ref == null)
      return Result.Nothing;

    var uuid = obj_ref.ObjectId;
    RhinoApp.WriteLine("The object's unique id is {0}", uuid.ToString());
    return Result.Success;
  }
}
