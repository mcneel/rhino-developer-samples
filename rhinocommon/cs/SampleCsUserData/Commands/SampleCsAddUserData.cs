using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  [System.Runtime.InteropServices.Guid("9d6fc690-8ca0-45c0-aea2-78328e51d9ca")]
  public class SampleCsAddUserData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsAddUserData"; }
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
      if (null != userdata)
      {
        RhinoApp.WriteLine("{0} = {1}", userdata.Description, userdata.Notes);
        return Result.Nothing;
      }

      var gs = new GetString();
      gs.SetCommandPrompt("Object notes");
      gs.GetLiteralString();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      userdata = new SampleCsUserDataObject 
      {
        Notes = gs.StringResult()
      };

      // To support undo, make a copy of the attributes
      // instead of modifying the attributes directly.
      var new_attributes = obj.Attributes.Duplicate();
      new_attributes.UserData.Add(userdata);
      doc.Objects.ModifyAttributes(obj, new_attributes, true);

      return Result.Success;
    }
  }
}
