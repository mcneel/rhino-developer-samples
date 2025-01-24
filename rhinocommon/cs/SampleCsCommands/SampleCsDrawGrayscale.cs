using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Input;
using System;
using System.Drawing;

namespace SampleCsCommands
{
  public class SampleCsDrawGrayscale : Command
  {
    public override string EnglishName => "SampleCsDrawGrayscale";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      SampleCsDrawGrayscaleConduit conduit = new SampleCsDrawGrayscaleConduit(view.ActiveViewportID)
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
    protected Guid ViewportId { get; }

    /// <summary>
    /// Called before every object in the scene is drawn
    /// </summary>
    protected override void PreDrawObject(DrawObjectEventArgs e)
    {
      if (e.Display.Viewport.Id == ViewportId)
      {
        Color old_color = e.Display.DisplayPipelineAttributes.ObjectColor;
        Color new_color = ConvertToGrayscale(old_color);
        // Works for curves
        e.Display.DisplayPipelineAttributes.ObjectColor = new_color;
      }
    }

    /// <summary>
    /// A common RGB color to grayscale formula
    /// </summary>
    static Color ConvertToGrayscale(Color color)
    {
      byte r = color.R;
      byte g = color.G;
      byte b = color.B;
      int luma = (int)(r * 0.3 + g * 0.59 + b * 0.11);
      Color gray = Color.FromArgb(luma, luma, luma);
      return gray;
    }
  }
}
