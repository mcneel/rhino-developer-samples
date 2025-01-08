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
      Rhino.Display.RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      System.Drawing.Size size = view.ActiveViewport.Size;

      Rhino.Input.Custom.GetInteger get = new Rhino.Input.Custom.GetInteger();
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

      string script = $"_-ViewportProperties _Size {size.Width} {size.Height} _Enter";
      RhinoApp.RunScript(script, false);

      return Result.Success;
    }
  }
}