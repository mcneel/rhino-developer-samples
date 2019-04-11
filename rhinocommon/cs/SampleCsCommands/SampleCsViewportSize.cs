using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  // Required to script the ViewportProperties command
  [CommandStyle(Style.ScriptRunner)]
  public class SampleCsViewportSize : Command
  {
    public override string EnglishName => "SampleCsViewportSize";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var size = view.ActiveViewport.Size;

      var get = new Rhino.Input.Custom.GetInteger();
      get.SetCommandPrompt("Viewport width in pixels");
      get.SetDefaultInteger(size.Width);
      get.SetLowerLimit(10, true);
      get.Get();
      if (get.CommandResult() != Result.Success)
        return get.CommandResult();

      size.Width = get.Number();

      get.SetCommandPrompt("Viewport height in pixels");
      get.SetDefaultInteger(size.Height);
      get.SetLowerLimit(10, true);
      get.Get();
      if (get.CommandResult() != Result.Success)
        return get.CommandResult();

      size.Height = get.Number();

      var script = $"_-ViewportProperties _Size {size.Width} {size.Height} _Enter";
      RhinoApp.RunScript(script, false);

      return Result.Success;
    }
  }
}