using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  internal static class SampleCsUserStringData
  {
    public static string Key { get { return "SampleCsUserStringData"; }}
    public static string Value { get { return "Hello Rhino!"; } }
  }

  /// <summary>
  /// Sets some sample user text
  /// </summary>
  [System.Runtime.InteropServices.Guid("E7278A9F-FCE9-4FC0-8AA0-85FFD378DE7E")]
  public class SampleCsSetUserText : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsSetUserText"; }
    }

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
  [System.Runtime.InteropServices.Guid("FF5DB119-7D95-44B2-B4CD-99EC8A068FDA")]
  public class SampleCsGetUserText : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsGetUserText"; }
    }

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
  [System.Runtime.InteropServices.Guid("19D14FD3-2867-4DE5-8E79-217EE2EB3E76")]
  public class SampleCsClearUserText : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsClearUserText"; }
    }

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
