using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  internal static class SampleCsUserStringData
  {
    public static string Key => "SampleCsUserStringData";
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
      var go = new GetObject();
      go.SetCommandPrompt("Select objects");
      go.GroupSelect = true;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (var i = 0; i < go.ObjectCount; i++)
      {
        var obj_ref = go.Object(i);
        var obj = obj_ref.Object();
        if (null != obj)
          obj.Attributes.SetUserString(SampleCsUserStringData.Key, SampleCsUserStringData.Value);
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
      var go = new GetObject();
      go.SetCommandPrompt("Select objects");
      go.GroupSelect = true;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (var i = 0; i < go.ObjectCount; i++)
      {
        var obj_ref = go.Object(i);
        var obj = obj_ref.Object();
        if (null != obj)
        {
          var value = obj.Attributes.GetUserString(SampleCsUserStringData.Key);
          if (!string.IsNullOrEmpty(value))
            RhinoApp.WriteLine(string.Format("<{0}> {1}", SampleCsUserStringData.Key, value));
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
      var go = new GetObject();
      go.SetCommandPrompt("Select objects");
      go.GroupSelect = true;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (var i = 0; i < go.ObjectCount; i++)
      {
        var obj_ref = go.Object(i);
        var obj = obj_ref.Object();
        if (null != obj)
          obj.Attributes.SetUserString(SampleCsUserStringData.Key, null);
      }

      return Result.Success;
    }    
  }

}
