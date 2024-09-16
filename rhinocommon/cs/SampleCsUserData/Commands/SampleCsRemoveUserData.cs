using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace SampleCsUserData.Commands
{
  public class SampleCsRemoveUserData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsRemoveUserData"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType filter = ObjectType.AnyObject;
      ObjRef objref;
      var rc = RhinoGet.GetOneObject("Select object", false, filter, out objref);
      if (rc != Result.Success || null == objref)
        return rc;

      var obj = objref.Object();
      if (null == obj)
        return Result.Failure;

      var ud = obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null != ud)
        obj.Attributes.UserData.Remove(ud);

      return Result.Success;
    }
  }
}
