using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  // Required for commands that call Rhino.RhinoApp.RunScript
  [CommandStyle(Style.ScriptRunner)]
  public class SampleCsOpen3dm : Command
  {
    public override string EnglishName => "SampleCsOpen3dm";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var filename = string.Empty;
      if (mode == Rhino.Commands.RunMode.Interactive)
        filename = RhinoGet.GetFileName(GetFileNameMode.OpenRhinoOnly, null, "Open", RhinoApp.MainWindowHandle());
      else
        RhinoGet.GetString("Name of Rhino file to open", false, ref filename);

      filename = filename.Trim();
      if (string.IsNullOrEmpty(filename))
        return Result.Cancel;

      if (!System.IO.File.Exists(filename))
      {
        RhinoApp.WriteLine("File not found.");
        return Result.Failure;
      }

      // Make sure to surround filename string with double-quote characters
      // in case the path contains spaces.
      var script = string.Format("_-Open \"{0}\"", filename);
      RhinoApp.RunScript(script, false);

      return Result.Success;
    }
  }
}
