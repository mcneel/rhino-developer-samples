using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  internal static class SampleStringData
  {
    public static string Key => nameof(SampleStringData);
    public static string Value => "Hello Rhino!";
  }

  /// <summary>
  /// Sets some sample user text
  /// </summary>
  public class SampleCsSetUserText : Command
  {
    public override string EnglishName => "SampleCsSetUserText";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject { GroupSelect = true };
      go.SetCommandPrompt("Select objects");
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var objref in go.Objects())
      {
        var obj = objref.Object();
        if (null != obj)
        {
          var attributes = obj.Attributes.Duplicate();
          attributes.SetUserString(SampleStringData.Key, SampleStringData.Value);
          doc.Objects.ModifyAttributes(objref, attributes, false);
        }
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// Gets some sample user text
  /// </summary>
  public class SampleCsGetUserText : Command
  {
    public override string EnglishName => "SampleCsGetUserText";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject { GroupSelect = true };
      go.SetCommandPrompt("Select objects");
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var objref in go.Objects())
      {
        var obj = objref.Object();
        if (null != obj)
        {
          var value = obj.Attributes.GetUserString(SampleStringData.Key);
          if (!string.IsNullOrEmpty(value))
            RhinoApp.WriteLine(string.Format("<{0}> {1}", SampleStringData.Key, value));
        }
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// Clears some sample user text
  /// </summary>
  public class SampleCsClearUserText : Command
  {
    public override string EnglishName => "SampleCsClearUserText";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject { GroupSelect = true };
      go.SetCommandPrompt("Select objects");
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var objref in go.Objects())
      {
        var obj = objref.Object();
        if (null != obj)
        {
          var attributes = obj.Attributes.Duplicate();
          attributes.SetUserString(SampleStringData.Key, null);
          doc.Objects.ModifyAttributes(objref, attributes, false);
        }
      }

      return Result.Success;
    }    
  }

}
