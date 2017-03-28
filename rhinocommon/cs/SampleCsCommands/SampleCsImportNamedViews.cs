using System;
using System.IO;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.FileIO;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("af0617ec-1ee8-4f81-b3e1-c078f5424176")]
  public class SampleCsImportNamedViews : Command
  {
    public SampleCsImportNamedViews()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsImportNamedViews"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      OpenFileDialog dialog = new OpenFileDialog();
      dialog.Filter = "Rhino 3D Models (*.3dm)|*.3dm";
      dialog.DefaultExt = "3dm";
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

      foreach (ViewInfo vi in f.NamedViews)
        doc.NamedViews.Add(vi);

      RhinoApp.WriteLine(string.Format("{0} named view(s) imported.", f.NamedViews.Count + 1));

      return Result.Success;
    }
  }
}
