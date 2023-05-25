using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace SampleCsUserData.Commands
{
  [System.Runtime.InteropServices.Guid("60fcd171-d0a4-4184-b2a4-22dc2e9c1ebe")]
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

      var attributes = obj.Attributes.Duplicate();
      var ud = obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null != ud)
      {
        attributes.UserData.Remove(ud);
        doc.Objects.ModifyAttributes(objref, attributes, false);
      }

      return Result.Success;
    }
  }
}
