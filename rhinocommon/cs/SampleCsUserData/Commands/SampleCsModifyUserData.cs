using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  [System.Runtime.InteropServices.Guid("59f0cc43-3019-41ae-b65b-87e1fbadfba3")]
  public class SampleCsModifyUserData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsModifyUserData"; }
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

      var gs = new GetString();
      gs.SetCommandPrompt("Modify object notes");
      gs.GetLiteralString();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      // To support undo, make a copy of the attributes
      // instead of modifying the attributes directly.
      var new_attributes = obj.Attributes.Duplicate();
      userdata = new_attributes.UserData.Find(typeof (SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null == userdata)
        return Result.Failure;

      userdata.Notes = gs.StringResult();
      doc.Objects.ModifyAttributes(obj, new_attributes, true);

      return Result.Success;
    }
  }
}
