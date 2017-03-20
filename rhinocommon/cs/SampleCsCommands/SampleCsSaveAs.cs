using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("d04298de-0d8c-4a0d-8820-29e7fd3e69bb")]
  public class SampleCsSaveAs : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsSaveAs"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string filename;

      if (mode == RunMode.Interactive)
      {
        var savefile = new SaveFileDialog
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
        var gs = new GetString();
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
