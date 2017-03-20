using System.Drawing;
using System.Drawing.Drawing2D;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsDrawTextConduit : DisplayConduit
  {
    const int X_GAP = 4; // pixels
    const int Y_GAP = 4; // pixels

    public SampleCsDrawTextConduit()
    {
    }

    protected override void DrawForeground(DrawEventArgs e)
    {
      int left, right, top, bottom, near, far;
      if (e.Viewport.GetScreenPort(out left, out right, out bottom, out top, out near, out far))
      {
        int width = right - left;
        int height = bottom - top;

        string str = "Hello Rhino!";
        Rectangle rect = e.Display.Measure2dText(str, new Point2d(0, 0), false, 0.0, 12, "Arial");

        if (rect.Width + (2*X_GAP) < width || rect.Height + (2*Y_GAP) < height)
        {
          // Cook up text location (lower right corner of viewport)
          Point2d point = new Point2d(right - rect.Width - X_GAP, bottom - Y_GAP);

          //e.Display.Draw2dRectangle(rect, Color.White, 1, Color.White);
          e.Display.Draw2dText(str, Color.Black, point, false, 12, "Arial");
        }
      }
    }
  }

  [System.Runtime.InteropServices.Guid("95bee6a7-a0f9-4da5-bc53-6f914373eee6")]
  public class SampleCsDrawText : Command
  {
    public SampleCsDrawText()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsDrawText"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var conduit = new SampleCsDrawTextConduit();
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
