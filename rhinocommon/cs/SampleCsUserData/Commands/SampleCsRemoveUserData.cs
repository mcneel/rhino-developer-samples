using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace SampleCsUserData.Commands
{
  public class SampleCsRemoveUserData : Command
  {
    public override string EnglishName => "SampleCsRemoveUserData";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType filter = ObjectType.AnyObject;
      var rc = RhinoGet.GetOneObject("Select object", false, filter, out ObjRef objref);
      if (rc != Result.Success || null == objref)
        return rc;

      var obj = objref.Object();
      if (null == obj)
        return Result.Failure;

      var attributes = obj.Attributes.Duplicate();
      if (attributes.UserData.Find(typeof(SampleCsUserDataObject)) is SampleCsUserDataObject ud)
      {
        attributes.UserData.Remove(ud);
        doc.Objects.ModifyAttributes(objref, attributes, false);
      }

      return Result.Success;
    }
  }
}
