using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsDrawArrow : Command
  {
    private readonly SampleCsArrowConduit m_conduit = new SampleCsArrowConduit();

    public override string EnglishName => "SampleCsDrawArrow";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      m_conduit.Enabled = !m_conduit.Enabled;
      doc.Views.Redraw();
      return Result.Success;
    }
  }

  internal class SampleCsArrowConduit : DisplayConduit
  {
    protected override void PostDrawObjects(DrawEventArgs e)
    {
      var points = new[] { new Point3d(2, 2, 0), new Point3d(2, -6, 0), new Point3d(5, -6, 0), new Point3d(5, 2, 0) };
      e.Display.DrawPolygon(points, Color.Black, false);

      var fill = Color.FromArgb(250, Color.White);
      var center = new Point3d(2, 2, 0);
      var off_x = new Point3d(3, 2, 0);
      var off_y = new Point3d(2, 3, 0);
      var radius = 3;

      e.Display.DrawPoint(center, PointStyle.RoundControlPoint, Color.Black, fill, radius, 1, 0, 0, true, true);

      var world_to_screen = e.Viewport.GetTransform(CoordinateSystem.World, CoordinateSystem.Screen);
      var center_screen = center;
      center_screen.Transform(world_to_screen);
      var off_screen = off_x;
      off_screen.Transform(world_to_screen);
      var xaxis = new Vector3d(off_screen.X - center_screen.X, off_screen.Y - center_screen.Y, 0);
      var angle = Vector3d.VectorAngle(Vector3d.XAxis, xaxis);
      if (xaxis.Y > 0)
        angle = -angle;
      e.Display.DrawPoint(center, PointStyle.ArrowTail, Color.Black, fill, 6, 1, 3, (float)angle, true, true);
      e.Display.DrawPoint(center, PointStyle.ArrowTail, Color.Black, fill, 6, 1, 3, (float)(angle + RhinoMath.ToRadians(180)), true, true);

      off_screen = off_y;
      off_screen.Transform(world_to_screen);
      var yaxis = new Vector3d(off_screen.X - center_screen.X, off_screen.Y - center_screen.Y, 0);
      angle = Vector3d.VectorAngle(Vector3d.YAxis, yaxis);
      if (yaxis.X < 0)
        angle = -angle;
      e.Display.DrawPoint(center, PointStyle.ArrowTail, Color.Black, fill, 6, 1, 3, (float)(angle + RhinoMath.ToRadians(90)), true, true);
      e.Display.DrawPoint(center, PointStyle.ArrowTail, Color.Black, fill, 6, 1, 3, (float)(angle + RhinoMath.ToRadians(270)), true, true);

      center = new Point3d(5, -2, 0);
      e.Display.DrawPoint(center, PointStyle.RoundControlPoint, Color.Black, fill, radius, 1, 0, 0, true, true);
      center_screen = center;
      center_screen.Transform(world_to_screen);
      off_screen = center;
      off_screen.X += 1;
      off_screen.Transform(world_to_screen);
      xaxis = new Vector3d(off_screen.X - center_screen.X, off_screen.Y - center_screen.Y, 0);
      angle = Vector3d.VectorAngle(Vector3d.XAxis, xaxis);
      if (xaxis.Y > 0)
        angle = -angle;
      e.Display.DrawPoint(center, PointStyle.ArrowTail, Color.Black, fill, 6, 1, 3, (float)angle, true, true);
      e.Display.DrawPoint(center, PointStyle.ArrowTail, Color.Black, fill, 6, 1, 3, (float)(angle + RhinoMath.ToRadians(180)), true, true);

      e.Display.DrawPoint(points[1], PointStyle.Pin, Color.Black, Color.PeachPuff, 15, 1, 5, 0, true, true);

      base.PostDrawObjects(e);
    }
  }
}