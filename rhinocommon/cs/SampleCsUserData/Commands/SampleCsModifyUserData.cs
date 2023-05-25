using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  public class SampleCsModifyUserData : Command
  {
    public override string EnglishName => "SampleCsModifyUserData";

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
        var gs = new GetString();
        gs.SetCommandPrompt("Modify object notes");
        gs.GetLiteralString();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        ud.Notes = gs.StringResult();
        doc.Objects.ModifyAttributes(objref, attributes, false);
      }

      return Result.Success;
    }
  }
}
