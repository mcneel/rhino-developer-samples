using System.Runtime.InteropServices;
using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsUnisolate : Command
  {
    public override string EnglishName => "SampleCsUnisolate";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      int index = SampleCsCommandsPlugIn.Instance.IsolateIndex - 1;
      if (index < 0)
      {
        RhinoApp.WriteLine("No isolated objects to unisolate.");
        SampleCsCommandsPlugIn.Instance.IsolateIndex = 0;
        return Result.Nothing;
      }

      var go = new GetOption();
      go.SetCommandPrompt("Choose unisolate option");
      int a_opt = go.AddOption("All");
      int p_opt = go.AddOption("Previous");
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var opt = go.Option();
      if (null == opt)
        return Result.Failure;

      string script = null;
      if (opt.Index == a_opt)
      {
        script = "_-Show _Enter";
        SampleCsCommandsPlugIn.Instance.IsolateIndex = 0;
      }
      else if (opt.Index == p_opt)
      {
        script = string.Format("_-Show {0}", index);
        SampleCsCommandsPlugIn.Instance.IsolateIndex--;
      }
      else
        return Result.Failure;

      RhinoApp.RunScript(script, false); 
      
      return Result.Success;
    }
  }
}
