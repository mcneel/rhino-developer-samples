using System;
using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Input;

namespace SampleCsCommands
{
  public class SampleCsDrawGrayscale : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsDrawGrayscale"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var conduit = new SampleCsDrawGrayscaleConduit(view.ActiveViewportID)
      {
        Enabled = true
      };
      doc.Views.Redraw();

      string out_str = null;
      RhinoGet.GetString("Press <Enter> to continue", true, ref out_str);

      conduit.Enabled = false;
      doc.Views.Redraw();

      return Result.Success;
    }
  }

  /// <summary>
  /// Our conduit
  /// </summary>
  class SampleCsDrawGrayscaleConduit : DisplayConduit
  {
    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsDrawGrayscaleConduit(Guid viewportId)
    {
      ViewportId = viewportId;
    }

    /// <summary>
    /// The ID of the viewport to be drawn grayscale
    /// </summary>
    protected Guid ViewportId { get; private set; }

    /// <summary>
    /// Called before every object in the scene is drawn
    /// </summary>
    protected override void PreDrawObject(DrawObjectEventArgs e)
    {
      if (e.Display.Viewport.Id == ViewportId)
      {
        var old_color = e.Display.DisplayPipelineAttributes.ObjectColor;
        var new_color = ConvertToGrayscale(old_color);
        // Works for curves
        e.Display.DisplayPipelineAttributes.ObjectColor = new_color;
      }
    }

    /// <summary>
    /// A common RGB color to grayscale formula
    /// </summary>
    static Color ConvertToGrayscale(Color color)
    {
      var r = color.R;
      var g = color.G;
      var b = color.B;
      var luma = (int)(r * 0.3 + g * 0.59 + b * 0.11);
      var gray = Color.FromArgb(luma, luma, luma);
      return gray;
    }
  }
}
