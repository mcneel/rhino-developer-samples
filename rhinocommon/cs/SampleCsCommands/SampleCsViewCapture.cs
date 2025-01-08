using Rhino;
using Rhino.Commands;
using Rhino.Display;
using System;
using System.IO;

namespace SampleCsCommands
{
  public class SampleCsViewCapture : Command
  {
    public override string EnglishName => "SampleCsViewCapture";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      ViewCapture view_capture = new ViewCapture
      {
        Width = view.ActiveViewport.Size.Width,
        Height = view.ActiveViewport.Size.Height,
        ScaleScreenItems = false,
        DrawAxes = false,
        DrawGrid = false,
        DrawGridAxes = false,
        TransparentBackground = true
      };

      System.Drawing.Bitmap bitmap = view_capture.CaptureToBitmap(view);
      if (null != bitmap)
      {
        string path = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
        string filename = Path.Combine(path, "SampleCsViewCapture.png");
        bitmap.Save(filename, System.Drawing.Imaging.ImageFormat.Png);
      }

      return Result.Success;
    }
  }
}
