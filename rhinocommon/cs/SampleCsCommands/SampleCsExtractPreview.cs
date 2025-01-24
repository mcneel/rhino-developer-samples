using Rhino;
using Rhino.Commands;
using System.IO;
using System.Windows.Forms;

namespace SampleCsCommands
{
  public class SampleCsExtractPreview : Command
  {
    public override string EnglishName => "SampleCsExtractPreview";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      OpenFileDialog dialog = new OpenFileDialog { Filter = @"Rhino 3D Models (*.3dm)|*.3dm", DefaultExt = "3dm" };
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
