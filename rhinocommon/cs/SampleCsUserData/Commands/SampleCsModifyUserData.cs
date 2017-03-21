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

      var ud = obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null != ud)
      {
        var gs = new GetString();
        gs.SetCommandPrompt("Modify object notes");
        gs.GetLiteralString();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        ud.Notes = gs.StringResult();
      }

      return Result.Success;
    }
  }
}
