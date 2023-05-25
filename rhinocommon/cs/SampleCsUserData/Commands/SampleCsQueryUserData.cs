using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace SampleCsUserData.Commands
{
  public class SampleCsQueryUserData : Command
  {
    public override string EnglishName => "SampleCsQueryUserData";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType filter = ObjectType.AnyObject;
      var rc = RhinoGet.GetOneObject("Select object", false, filter, out ObjRef objref);
      if (rc != Result.Success || null == objref)
        return rc;

      var obj = objref.Object();
      if (null == obj)
        return Result.Failure;

      if (obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) is SampleCsUserDataObject ud)
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      else
        RhinoApp.WriteLine("User data not found.");

      return Result.Success;
    }
  }
}
