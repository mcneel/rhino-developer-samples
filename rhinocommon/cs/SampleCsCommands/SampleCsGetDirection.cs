using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsGetDirection : Command
  {
    public override string EnglishName => "SampleCsGetDirection";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to draw from");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var pt = gp.Point();

      var gd = new GetDirection();
      gd.SetCommandPrompt("Point to draw to");
      gd.SetBasePoint(pt, true);
      gd.DrawArrowFromPoint(pt, true, true);
      gd.Get();
      if (gd.CommandResult() != Result.Success)
        return gd.CommandResult();

      // TODO...

      return Result.Success;
    }
  }

  /// <summary>
  /// GetPoint-derived class that allows for the drawing of a direction arrow.
  /// </summary>
  public class GetDirection : GetPoint
  {
    private Point3d m_draw_arrow_point = Point3d.Unset;
    private bool m_draw_arrow;
    private bool m_show_distance;
    private bool m_show_simple_arrow;

    /// <summary>
    /// Use DrawLineFromPoint() if you want a dynamic direction arrow drawn from a point to the point being picked.
    /// </summary>
    /// <param name="startPoint">The line is drawn from startPoint to the point being picked. If the base point has not been set, then it is set to startPoint.</param>
    /// <param name="showDistanceInStatusBar">if true, the distance from the basePoint to the point begin picked is shown in the status bar.</param>
    /// <param name="showSimpleArrow">if true, a line with a simple arrowhead is drawn. If false, a line with a fancy arrowhead is drawn.</param>
    public void DrawArrowFromPoint(Point3d startPoint, bool showDistanceInStatusBar, bool showSimpleArrow)
    {
      m_draw_arrow = true;
      m_show_distance = showDistanceInStatusBar;
      m_show_simple_arrow = showSimpleArrow;
      m_draw_arrow_point = startPoint;
      if (startPoint.IsValid)
      {
        SetBasePoint(startPoint, showDistanceInStatusBar);
        AddSnapPoint(startPoint);
      }
    }

    /// <summary>
    /// Controls drawing of dynamic a direction arrow from the start point.
    /// </summary>
    /// <param name="enable">if true, a dynamic direction arrow is drawn from the DrawArrowFromPoint startPoint to the point being picked.</param>
    public void EnableDrawArrowFromPoint(bool enable)
    {
      m_draw_arrow = enable;
    }

    /// <summary>
    /// GetPoint.OnMouseMove override
    /// </summary>
    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      if (m_draw_arrow && m_show_distance && m_draw_arrow_point.IsValid)
        Rhino.UI.StatusBar.SetDistancePane(m_draw_arrow_point.DistanceTo(e.Point));
      base.OnMouseMove(e);
    }

    /// <summary>
    /// GetPoint.OnDynamicDraw override
    /// </summary>
    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (m_draw_arrow && m_draw_arrow_point.IsValid)
      {
        var line = new Line(m_draw_arrow_point, e.CurrentPoint);
        if (line.IsValid)
        {
          if (m_show_simple_arrow)
          {
            var direction = line.To - line.From;
            direction.Unitize();
            e.Display.DrawLine(line, DynamicDrawColor);
            e.Display.DrawDirectionArrow(line.To, direction, DynamicDrawColor);
          }
          else
          {
            e.Display.DrawArrow(line, DynamicDrawColor);
          }
        }
      }
      base.OnDynamicDraw(e);
    }
  }

}
