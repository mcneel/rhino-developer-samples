using System;
using System.IO;
using Rhino;
using Rhino.Commands;
using Rhino.Display;

namespace SampleCsCommands
{
  public class SampleCsViewCapture : Command
  {
    public override string EnglishName => "SampleCsViewCapture";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var view_capture = new ViewCapture
      {
        Width = view.ActiveViewport.Size.Width,
        Height = view.ActiveViewport.Size.Height,
        ScaleScreenItems = false,
        DrawAxes = false,
        DrawGrid = false,
        DrawGridAxes = false,
        TransparentBackground = true
      };

      var bitmap = view_capture.CaptureToBitmap(view);
      if (null != bitmap)
      {
        var path = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
        var filename = Path.Combine(path, "SampleCsViewCapture.png");
        bitmap.Save(filename, System.Drawing.Imaging.ImageFormat.Png);
      }


      return Result.Success;
    }
  }
}