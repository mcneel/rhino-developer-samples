using Rhino;
using Rhino.Commands;
using Rhino.Display;
using System;
using System.Drawing;

namespace SampleCsCommands
{
  public class SampleCsViewSize : Command
  {
    public override string EnglishName => "SampleCsViewSize";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      Rectangle rect = view.ScreenRectangle;
      int width_in_pixels = rect.Width;
      int height_in_pixels = rect.Height;

      Graphics graphics = Graphics.FromHwnd(IntPtr.Zero);
      float pixels_per_inch_x = graphics.DpiX;
      float pixels_per_inch_y = graphics.DpiY;

      float width_in_inches = width_in_pixels / pixels_per_inch_x;
      float height_in_inches = height_in_pixels / pixels_per_inch_y;

      const double mm_per_inch = 25.4;
      double width_in_mm = width_in_inches * mm_per_inch;
      double height_in_mm = height_in_inches * mm_per_inch;

      string name = view.ActiveViewport.Name;
      RhinoApp.WriteLine(string.Format("{0} view width: {1} mm", name, width_in_mm));
      RhinoApp.WriteLine(string.Format("{0} view height: {1} mm", name, height_in_mm));

      return Result.Success;
    }
  }
}
