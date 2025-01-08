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
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select objects");
      go.SubObjectSelect = false;
      go.ReferenceObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      string defaultName = null;
      foreach (Rhino.DocObjects.ObjRef objRef in go.Objects())
      {
        Rhino.DocObjects.RhinoObject rhObj = objRef.Object();
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

      GetString gs = new GetString();
      gs.SetCommandPrompt("Object name");
      gs.SetDefaultString(defaultName);
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      string newName = gs.StringResult();
      newName = newName.Trim();

      if (defaultName.Equals(newName))
        return Result.Nothing;

      foreach (Rhino.DocObjects.ObjRef objRef in go.Objects())
      {
        Rhino.DocObjects.RhinoObject rhObj = objRef.Object();
        if (null == rhObj)
          return Result.Failure;

        if (!newName.Equals(rhObj.Attributes.Name))
        {
          Rhino.DocObjects.ObjectAttributes attributes = rhObj.Attributes.Duplicate();
          attributes.Name = newName;
          doc.Objects.ModifyAttributes(rhObj, attributes, false);
        }
      }

      return Result.Success;
    }
  }
}