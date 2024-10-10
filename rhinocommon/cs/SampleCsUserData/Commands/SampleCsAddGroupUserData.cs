using Rhino;
using Rhino.Commands;

namespace SampleCsUserData.Commands
{
  public class SampleCsAddGroupUserData : Command
  {
    public override string EnglishName => "SampleCsAddGroupUserData";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var group = doc.Groups.FindIndex(0);
      if (null == group)
      {
        RhinoApp.WriteLine("No groups found in the document.");
        return Result.Success;
      }

      var ud = group.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null == ud)
      {
        ud = new SampleCsUserDataObject { Notes = "Hello Rhino Group!" };
        group.UserData.Add(ud);
        RhinoApp.WriteLine("{0} added", ud.Description);
      }
      else
      {
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      }

      return Result.Success;
    }
  }
}