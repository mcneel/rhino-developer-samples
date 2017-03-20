using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [
    System.Runtime.InteropServices.Guid("6f614237-84f1-4441-a410-d946cedd825e"),
    // Required for commands that call Rhino.RhinoApp.RunScript
    Rhino.Commands.CommandStyle(Rhino.Commands.Style.ScriptRunner)
  ]
  public class SampleCsOpen3dm : Command
  {
    public SampleCsOpen3dm()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsOpen3dm"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string filename = string.Empty;
      if (mode == Rhino.Commands.RunMode.Interactive)
        filename = Rhino.Input.RhinoGet.GetFileName(Rhino.Input.Custom.GetFileNameMode.OpenRhinoOnly, null, "Open", Rhino.RhinoApp.MainWindow());
      else
        Rhino.Input.RhinoGet.GetString("Name of Rhino file to open", false, ref filename);

      filename = filename.Trim();
      if (string.IsNullOrEmpty(filename))
        return Rhino.Commands.Result.Cancel;

      if (!System.IO.File.Exists(filename))
      {
        Rhino.RhinoApp.WriteLine("File not found.");
        return Rhino.Commands.Result.Failure;
      }

      // Make sure to surround filename string with double-quote characters
      // in case the path contains spaces.
      string script = string.Format("_-Open \"{0}\"", filename);
      Rhino.RhinoApp.RunScript(script, false);

      return Rhino.Commands.Result.Success;
    }
  }
}
