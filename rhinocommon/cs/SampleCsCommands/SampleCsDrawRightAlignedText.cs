using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsDrawRightAlignedTextConduit : DisplayConduit
  {
    private const int X_GAP = 4; // pixels
    private const int Y_GAP = 4; // pixels

    private readonly string[] m_strings = new string[5];

    public SampleCsDrawRightAlignedTextConduit()
    {
      m_strings[0] = @"Robert McNeel & Associates";
      m_strings[1] = @"3670 Woodland Park Avenue North";
      m_strings[2] = @"Seattle, WA 98103 USA";
      m_strings[3] = @"(206) 545-7000";
      m_strings[4] = @"http://www.rhino3d.com";
    }

    protected override void DrawForeground(DrawEventArgs e)
    {
      if (e.Viewport.GetScreenPort(out int left, out int right, out int bottom, out int top, out int near, out int far))
      {
        for (var i = m_strings.Length - 1; i >= 0; i--)
        {
          var rect = e.Display.Measure2dText(m_strings[i], new Point2d(0, 0), false, 0.0, 12, "Arial");
          var width = System.Math.Abs(rect.Width);
          var height = System.Math.Abs(rect.Height);
          var point = new Point2d(right - width - X_GAP, bottom - height - Y_GAP);
          e.Display.Draw2dText(m_strings[i], Color.Black, point, false, 12, "Arial");
          bottom = bottom - height - Y_GAP;
        }
      }
    }
  }

  public class SampleCsDrawRightAlignedText : Command
  {
    public override string EnglishName => "SampleCsDrawRightAlignedText";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var conduit = new SampleCsDrawRightAlignedTextConduit();
      conduit.Enabled = true;
      doc.Views.Redraw();

      var gs = new GetString();
      gs.SetCommandPrompt("Press <Enter> to continue");
      gs.AcceptNothing(true);
      gs.Get();

      conduit.Enabled = false;
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
