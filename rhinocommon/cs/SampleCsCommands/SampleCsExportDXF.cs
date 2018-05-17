using System.IO;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  // Make sure to add the ScriptRunner command style attribute
  [CommandStyle(Style.ScriptRunner)]
  public class SampleCsExportDXF : Command
  {
    public override string EnglishName => "SampleCsExportDXF";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select objects to export");
      go.GroupSelect = true;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      string path;
      if (mode == RunMode.Interactive)
      {
        var savefile = new SaveFileDialog
        {
          FileName = @"Untitled.dxf",
          Filter = @"AutoCAD Drawing Exchange (*.dxf)|*.dxf||"
        };
        if (savefile.ShowDialog() != DialogResult.OK)
          return Result.Cancel;

        path = savefile.FileName.Trim();
      }
      else
      {
        var gs = new GetString();
        gs.SetCommandPrompt("Name of AutoCAD Drawing Exchange file to save");
        gs.Get();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        path = gs.StringResult().Trim();
      }

      if (string.IsNullOrEmpty(path))
        return Result.Nothing;

      // Ensure the path has a ".dxf" extension
      if (!Path.HasExtension(path))
        path = Path.ChangeExtension(path, ".dxf");

      // Script Rhino's Export command. Note, in case the path
      // string contains spaces, we will want to surround the string
      // with double-quote characters so the command line parser
      // will deal with it property.
      var script = $"_-Export \"{path}\" _Enter";
      RhinoApp.RunScript(script, false);

      return Result.Success;
    }
  }
}