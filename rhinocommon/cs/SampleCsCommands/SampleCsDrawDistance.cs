using System;
using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsDrawDistance : Command
  {
    public SampleCsDrawDistance()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsDrawDistance"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Point3d basePt;
      Result rc = RhinoGet.GetPoint("Start of line", false, out basePt);
      if (rc != Result.Success)
        return rc;

      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("End of line");
      gp.SetBasePoint(basePt, true);
      gp.DrawLineFromPoint(basePt, true);
      gp.DynamicDraw += new EventHandler<GetPointDrawEventArgs>(gp_DynamicDraw);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      Point3d endPt = gp.Point();
      Rhino.Geometry.Vector3d vector = endPt - basePt;
      if (vector.Length > doc.ModelAbsoluteTolerance)
      {
        Line line = new Line(basePt, endPt);
        doc.Objects.AddLine(line);
        doc.Views.Redraw();
      }

      return Result.Success;
    }

    void gp_DynamicDraw(object sender, GetPointDrawEventArgs e)
    {
      Point3d basePt;
      if (e.Source.TryGetBasePoint(out basePt))
      {
        // Format distance as string
        double distance = basePt.DistanceTo(e.CurrentPoint);
        string text = string.Format("{0:0.000}", distance);

        // Get world-to-screen coordinate transformation
        Transform xform = e.Viewport.GetTransform(Rhino.DocObjects.CoordinateSystem.World, Rhino.DocObjects.CoordinateSystem.Screen);

        // Transform point from world to screen coordinates
        Point3d screenPt = xform * e.CurrentPoint;

        // Offset point so text does not overlap cursor
        screenPt.X += 5.0;
        screenPt.Y -= 5.0;

        // Draw the string 
        e.Display.Draw2dText(text, Color.Black, new Point2d(screenPt.X, screenPt.Y), false);
      }
    }
  }
}
