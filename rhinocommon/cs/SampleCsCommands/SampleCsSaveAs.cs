using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using System.Windows.Forms;

namespace SampleCsCommands
{
  public class SampleCsSaveAs : Command
  {
    public override string EnglishName => "SampleCsSaveAs";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string filename;

      if (mode == RunMode.Interactive)
      {
        SaveFileDialog savefile = new SaveFileDialog
        {
          FileName = "Untitled.txt",
          Filter = @"Text files (*.txt)|*.txt|All files (*.*)|*.*"
        };
        if (savefile.ShowDialog() != DialogResult.OK)
          return Result.Cancel;

        filename = savefile.FileName.Trim();
      }
      else
      {
        GetString gs = new GetString();
        gs.SetCommandPrompt("Name of text file to save");
        gs.Get();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        filename = gs.StringResult().Trim();
      }

      if (string.IsNullOrEmpty(filename))
        return Result.Nothing;

      // TODO: do something with 'filename'

      RhinoApp.WriteLine(filename);

      return Result.Success;
    }
  }
}
