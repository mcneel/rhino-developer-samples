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

      var userdata = obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null == userdata)
      {
        RhinoApp.WriteLine("SampleCsUserData not found.");
        return Result.Nothing;
      }

      // To support undo, make a copy of the attributes
      // instead of modifying the attributes directly.
      var new_attributes = obj.Attributes.Duplicate();
      userdata = new_attributes.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null == userdata)
        return Result.Failure;

      new_attributes.UserData.Remove(userdata);
      doc.Objects.ModifyAttributes(obj, new_attributes, true);

      return Result.Success;
    }
  }
}
