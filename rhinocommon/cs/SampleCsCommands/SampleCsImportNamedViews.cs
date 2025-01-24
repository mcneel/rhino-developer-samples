using Rhino;
using Rhino.Commands;
using Rhino.FileIO;
using System.IO;
using System.Windows.Forms;

namespace SampleCsCommands
{
  public class SampleCsImportNamedViews : Command
  {
    public override string EnglishName => "SampleCsImportNamedViews";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      OpenFileDialog dialog = new OpenFileDialog
      {
        Filter = @"Rhino 3D Models (*.3dm)|*.3dm",
        DefaultExt = "3dm"
      };
      DialogResult rc = dialog.ShowDialog();
      if (rc != DialogResult.OK)
        return Result.Cancel;

      string filename = dialog.FileName;
      if (string.IsNullOrEmpty(filename) || !File.Exists(filename))
        return Result.Failure;

      File3dm f = File3dm.Read(filename);
      if (0 == f.NamedViews.Count)
      {
        RhinoApp.WriteLine("No named views to import.");
        return Result.Nothing;
      }

      foreach (Rhino.DocObjects.ViewInfo vi in f.NamedViews)
        doc.NamedViews.Add(vi);

      RhinoApp.WriteLine($"{f.NamedViews.Count + 1} named view(s) imported.");

      return Result.Success;
    }
  }
}
