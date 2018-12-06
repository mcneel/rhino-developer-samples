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
      ObjRef objref;
      var rc = RhinoGet.GetOneObject("Select object", false, filter, out objref);
      if (rc != Result.Success || null == objref)
        return rc;

      var obj = objref.Object();
      if (null == obj)
        return Result.Failure;

      var ud = obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null == ud)
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

        obj.Attributes.UserData.Add(ud);
      }
      else
      {
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      }

      return Result.Success;
    }
  }
}
