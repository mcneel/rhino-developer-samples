using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace examples_cs
{
  public class GetUuidCommand : Command
  {
    public override string EnglishName { get { return "csGetUUID"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
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
}