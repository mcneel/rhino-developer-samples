using System.IO;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.FileIO;

namespace SampleCsCommands
{
  public class SampleCsImportNamedViews : Command
  {
    public override string EnglishName => "SampleCsImportNamedViews";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var dialog = new OpenFileDialog
      {
        Filter = @"Rhino 3D Models (*.3dm)|*.3dm",
        DefaultExt = "3dm"
      };
      var rc = dialog.ShowDialog();
      if (rc != DialogResult.OK)
        return Result.Cancel;

      var filename = dialog.FileName;
      if (string.IsNullOrEmpty(filename) || !File.Exists(filename))
        return Result.Failure;

      var f = File3dm.Read(filename);
      if (0 == f.NamedViews.Count)
      {
        RhinoApp.WriteLine("No named views to import.");
        return Result.Nothing;
      }

      foreach (var vi in f.NamedViews)
        doc.NamedViews.Add(vi);

      RhinoApp.WriteLine($"{f.NamedViews.Count + 1} named view(s) imported.");

      return Result.Success;
    }
  }
}
