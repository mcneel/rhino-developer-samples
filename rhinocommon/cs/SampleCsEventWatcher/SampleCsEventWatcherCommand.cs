using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsEventWatcher
{
  /// <summary>
  /// SampleCsEventWatcher command
  /// </summary>
  public class SampleCsEventWatcherCommand : Command
  {
    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsEventWatcherCommand()
    {
      // Rhino only creates one instance of each command class,
      // so it is safe to store a reference in a static property.
      Instance = this;
    }

    /// <summary>
    /// The one and only instance of this command.
    /// </summary>
    public static SampleCsEventWatcherCommand Instance
    {
      get;
      private set;
    }

    /// <returns>
    /// The command name as it appears on the Rhino command line.
    /// </returns>
    public override string EnglishName
    {
      get { return "SampleCsEventWatcher"; }
    }

    /// <summary>
    /// Called by Rhino to run the command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      bool enabled = SampleCsEventHandlers.Instance.IsEnabled;
      string prompt = enabled ? "Event watcher is enabled. New value" : "Event watcher is disabled. New value";

      GetOption go = new GetOption();
      go.SetCommandPrompt(prompt);
      go.AcceptNothing(true);

      int d_option = go.AddOption("Disable");
      int e_option = go.AddOption("Enable");
      int t_option = go.AddOption("Toggle");

      GetResult res = go.Get();
      if (res == GetResult.Nothing)
        return Result.Success;
      if (res != GetResult.Option)
        return Result.Cancel;

      CommandLineOption option = go.Option();
      if (null == option)
        return Result.Failure;

      if (d_option == option.Index)
      {
        if (enabled)
          SampleCsEventHandlers.Instance.Enable(false);
      }
      else if (e_option == option.Index)
      {
        if (!enabled)
          SampleCsEventHandlers.Instance.Enable(true);
      }
      else if (t_option == option.Index)
      {
        SampleCsEventHandlers.Instance.Enable(!enabled);
      }

      return Result.Success;
    }
  }
}
