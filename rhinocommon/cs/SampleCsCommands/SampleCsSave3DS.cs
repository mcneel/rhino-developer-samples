using System.IO;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  // Make sure to add the ScriptRunner command style attribute
  [CommandStyle(Style.ScriptRunner)]
  public class SampleCsSave3DS : Command
  {
    public override string EnglishName => "SampleCsSave3DS";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string path;
      if (mode == RunMode.Interactive)
      {
        var savefile = new SaveFileDialog
        {
          FileName = "Untitled.3ds",
          Filter = @"3D Studio (*.3ds)|*.3ds||"
        };
        if (savefile.ShowDialog() != DialogResult.OK)
          return Result.Cancel;

        path = savefile.FileName.Trim();
      }
      else
      {
        var gs = new GetString();
        gs.SetCommandPrompt("Name of 3D Studio file to save");
        gs.Get();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        path = gs.StringResult().Trim();
      }

      if (string.IsNullOrEmpty(path))
        return Result.Nothing;

      // Ensure the path has a ".3ds" extension
      if (!Path.HasExtension(path))
        path = Path.ChangeExtension(path, ".3ds");

      // Script Rhino's SaveAs command. Note, in case the path
      // string contains spaces, we will want to surround the string
      // with double-quote characters so the command line parser
      // will deal with it property.
      var script = $"_-SaveAs \"{path}\" _Enter";
      RhinoApp.RunScript(script, false);

      return Result.Success;
    }
  }
}