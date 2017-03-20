using System;
using System.Drawing;
using System.Runtime.InteropServices;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsDrawRightAlignedTextConduit : DisplayConduit
  {
    const int X_GAP = 4; // pixels
    const int Y_GAP = 4; // pixels

    readonly string[] m_strings = new string[5];

    public SampleCsDrawRightAlignedTextConduit()
    {
      m_strings[0] = "Robert McNeel & Associates";
      m_strings[1] = "3670 Woodland Park Avenue North";
      m_strings[2] = "Seattle, WA 98103 USA";
      m_strings[3] = "(206) 545-7000";
      m_strings[4] = "http://www.rhino3d.com";
    }

    protected override void DrawForeground(DrawEventArgs e)
    {
      int left, right, top, bottom, near, far;
      if (e.Viewport.GetScreenPort(out left, out right, out bottom, out top, out near, out far))
      {
        var screen_width = right - left;
        var screen_height = bottom - top;

        for (var i = m_strings.Length - 1; i >= 0; i--)
        {
          var str = m_strings[i];
          var rect = e.Display.Measure2dText(str, new Point2d(0, 0), false, 0.0, 12, "Arial");

          if (rect.Width + (2*X_GAP) < screen_width || rect.Height + (2*Y_GAP) < screen_height)
          {
            var point = new Point2d(right - rect.Width - X_GAP, bottom - Y_GAP);

            e.Display.Draw2dText(str, Color.Black, point, false, 12, "Arial");

            bottom = bottom - Y_GAP - rect.Height;
          }
        }
      }
    }
  }

  [System.Runtime.InteropServices.Guid("7cbc6972-069b-45a2-8fac-3958ab7ad4ae")]
  public class SampleCsDrawRightAlignedText : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsDrawRightAlignedText"; }
    }

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
