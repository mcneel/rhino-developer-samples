using System;
using System.Drawing;
using System.Linq.Expressions;
using System.Runtime.InteropServices;
using Rhino;
using Rhino.Commands;
using Rhino.Display;

namespace SampleCsCommands
{
  public class SampleCsViewSize : Command
  {
    public SampleCsViewSize()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsViewSize"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var rect = view.ScreenRectangle;
      var width_in_pixels = rect.Width;
      var height_in_pixels = rect.Height;

      var graphics = Graphics.FromHwnd(IntPtr.Zero);
      var pixels_per_inch_x = graphics.DpiX;
      var pixels_per_inch_y = graphics.DpiY;

      var width_in_inches = width_in_pixels / pixels_per_inch_x;
      var height_in_inches = height_in_pixels / pixels_per_inch_y;

      const double mm_per_inch = 25.4;
      var width_in_mm = width_in_inches * mm_per_inch;
      var height_in_mm = height_in_inches * mm_per_inch;

      var name = view.ActiveViewport.Name;
      RhinoApp.WriteLine(string.Format("{0} view width: {1} mm", name, width_in_mm));
      RhinoApp.WriteLine(string.Format("{0} view height: {1} mm", name, height_in_mm));

      return Result.Success;
    }
  }
}
