using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;
using System;
using System.Windows;
using System.Windows.Controls;

namespace examples_cs
{
  public class ExtractThumbnailCommand : Command
  {
    public override string EnglishName { get { return "csExtractThumbnail"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gf = RhinoGet.GetFileName(GetFileNameMode.OpenImage, "*.3dm", "select file", null);
      if (gf == string.Empty || !System.IO.File.Exists(gf))
        return Result.Cancel;

      var bitmap = Rhino.FileIO.File3dm.ReadPreviewImage(gf);
      // convert System.Drawing.Bitmap to BitmapSource
      var image_source = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(bitmap.GetHbitmap(), IntPtr.Zero,
        Int32Rect.Empty, System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions());

      // show in WPF window
      var window = new Window();
      var image = new Image {Source = image_source};
      window.Content = image;
      window.Show();

      return Result.Success;
    }
  }
}