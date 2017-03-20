using System;
using System.IO;
using System.Windows;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("26f27d4f-f596-4de8-bfdf-0e274c9bd808")]
  public class SampleCsExtractPreview : Command
  {
    public SampleCsExtractPreview()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsExtractPreview"; }
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

      System.Drawing.Bitmap bitmap = Rhino.RhinoDoc.ExtractPreviewImage(filename);
      if (null != bitmap)
      {
        filename = Path.ChangeExtension(filename, "png");
        bitmap.Save(filename, System.Drawing.Imaging.ImageFormat.Png);
      }

      return Result.Success;
    }
  }
}
