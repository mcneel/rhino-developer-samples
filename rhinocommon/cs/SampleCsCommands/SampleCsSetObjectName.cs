using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsSetObjectName : Command
  {
    public override string EnglishName => "SampleCsSetObjectName";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select objects");
      go.SubObjectSelect = false;
      go.ReferenceObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      string defaultName = null;
      foreach (var objRef in go.Objects())
      {
        var rhObj = objRef.Object();
        if (null == rhObj)
          return Result.Failure;

        if (string.IsNullOrEmpty(defaultName))
          defaultName = rhObj.Attributes.Name;
        else if (!defaultName.Equals(rhObj.Attributes.Name))
        {
          defaultName = "varies";
          break;
        }
      }

      var gs = new GetString();
      gs.SetCommandPrompt("Object name");
      gs.SetDefaultString(defaultName);
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var newName = gs.StringResult();
      newName = newName.Trim();

      if (defaultName.Equals(newName))
        return Result.Nothing;

      foreach (var objRef in go.Objects())
      {
        var rhObj = objRef.Object();
        if (null == rhObj)
          return Result.Failure;

        if (!newName.Equals(rhObj.Attributes.Name))
        {
          var attributes = rhObj.Attributes.Duplicate();
          attributes.Name = newName;
          doc.Objects.ModifyAttributes(rhObj, attributes, false);
        }
      }

      return Result.Success;
    }
  }
}