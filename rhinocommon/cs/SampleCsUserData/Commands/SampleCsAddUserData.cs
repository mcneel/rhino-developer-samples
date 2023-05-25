using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  public class SampleCsAddUserData : Command
  {
    public override string EnglishName => "SampleCsAddUserData";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType filter = ObjectType.AnyObject;
      var rc = RhinoGet.GetOneObject("Select object", false, filter, out ObjRef objref);
      if (rc != Result.Success || null == objref)
        return rc;

      var obj = objref.Object();
      if (null == obj)
        return Result.Failure;

      if (!(obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) is SampleCsUserDataObject ud))
      {
        var gs = new GetString();
        gs.SetCommandPrompt("Object notes");
        gs.GetLiteralString();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        ud = new SampleCsUserDataObject
        {
          Notes = gs.StringResult()
        };

        var attributes = obj.Attributes.Duplicate();
        attributes.UserData.Add(ud);
        doc.Objects.ModifyAttributes(objref, attributes, false);
      }
      else
      {
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      }

      return Result.Success;
    }
  }
}
